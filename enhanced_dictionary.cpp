#include "enhanced_dictionary.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <codecvt>
#include <locale>

EnhancedDictionary::EnhancedDictionary() : totalSearches(0), cacheHits(0), userWordsFile("user_words.txt") {
    generateMassiveWordDatabase();
    loadUserWords(); // Load previously saved user words
}

std::string EnhancedDictionary::processWord(const std::string& word) const {
    std::string result = word;
    result.erase(std::remove_if(result.begin(), result.end(),
        [](unsigned char c) {
            return std::ispunct(c) && c != '\'' && c != '-';
        }), result.end());
    std::transform(result.begin(), result.end(),
        result.begin(), [](unsigned char c) { return std::tolower(c); });
    return result;
}

bool EnhancedDictionary::addWord(const MultiWordEntry& entry) {
    if (!validateWordEntry(entry)) {
        return false;
    }

    std::lock_guard<std::mutex> lock(dataMutex);
    std::string processedWord = processWord(entry.word);
    MultiWordEntry processedEntry = entry;
    processedEntry.word = entry.word;
    processedEntry.frequency = std::max(1, entry.frequency);

    trie.insert(processedEntry);
    hashTable[processedWord] = processedEntry;

    auto it = std::find_if(wordList.begin(), wordList.end(),
        [&processedWord, this](const MultiWordEntry& e) {
            return processWord(e.word) == processedWord;
        });

    if (it == wordList.end()) {
        wordList.push_back(processedEntry);
    } else {
        *it = processedEntry;
    }

    searchCache.erase(processedWord);
    // NEW: Save to persistent storage
    saveUserWords();
    return true;
}

bool EnhancedDictionary::addWord(const std::string& word, const std::string& meaning,
    const std::string& arabic, const std::string& pos) {
    MultiWordEntry entry(word, meaning, arabic, pos);
    return addWord(entry);
}

bool EnhancedDictionary::addMeaningToWord(const std::string& word, const std::string& meaning,
    const std::string& arabic, const std::string& pos) {
    if (word.empty() || meaning.empty()) return false;

    std::lock_guard<std::mutex> lock(dataMutex);
    std::string processedWord = processWord(word);

    auto it = hashTable.find(processedWord);
    if (it != hashTable.end()) {
        it->second.addMeaning(meaning, arabic, pos);

        // Update in wordList too
        auto listIt = std::find_if(wordList.begin(), wordList.end(),
            [&processedWord, this](const MultiWordEntry& e) {
                return processWord(e.word) == processedWord;
            });
        if (listIt != wordList.end()) {
            *listIt = it->second;
        }

        searchCache.erase(processedWord);
        saveUserWords();
        return true;
    }

    // Word doesn't exist, create new entry
    MultiWordEntry newEntry(word, meaning, arabic, pos);
    return addWord(newEntry);
}

// Enhanced: Delete specific meaning or entire word
bool EnhancedDictionary::deleteWordMeaning(const std::string& word, int meaningIndex) {
    if (word.empty()) return false;

    std::lock_guard<std::mutex> lock(dataMutex);
    std::string processedWord = processWord(word);

    auto hashIt = hashTable.find(processedWord);
    if (hashIt == hashTable.end()) {
        return false; // Word not found
    }

    if (meaningIndex == -1) {
        // Delete entire word
        hashTable.erase(hashIt);

        // Remove from word list
        auto listIt = std::find_if(wordList.begin(), wordList.end(),
            [&processedWord, this](const MultiWordEntry& e) {
                return processWord(e.word) == processedWord;
            });
        if (listIt != wordList.end()) {
            wordList.erase(listIt);
        }

        searchCache.erase(processedWord);
        saveUserWords();
        return true;
    } else {
        // Delete specific meaning
        if (hashIt->second.removeMeaning(meaningIndex)) {
            // Update in wordList too
            auto listIt = std::find_if(wordList.begin(), wordList.end(),
                [&processedWord, this](const MultiWordEntry& e) {
                    return processWord(e.word) == processedWord;
                });
            if (listIt != wordList.end()) {
                *listIt = hashIt->second;
            }

            // If no meanings left, delete the word entirely
            if (hashIt->second.meanings.empty()) {
                hashTable.erase(hashIt);
                if (listIt != wordList.end()) {
                    wordList.erase(listIt);
                }
            }

            searchCache.erase(processedWord);
            saveUserWords();
            return true;
        }
    }

    return false;
}

std::vector<std::string> EnhancedDictionary::getWordMeanings(const std::string& word) {
    MultiWordEntry entry = getWordEntry(word);
    return entry.meanings;
}

MultiWordEntry EnhancedDictionary::getWordEntry(const std::string& word) {
    if (word.empty()) return MultiWordEntry();
    std::string processedWord = processWord(word);

    {
        std::lock_guard<std::mutex> lock(dataMutex);
        totalSearches++;
        auto cacheIt = searchCache.find(processedWord);
        if (cacheIt != searchCache.end()) {
            cacheHits++;
            return cacheIt->second;
        }
    }

    {
        std::lock_guard<std::mutex> lock(dataMutex);
        auto it = hashTable.find(processedWord);
        if (it != hashTable.end()) {
            addToCache(processedWord, it->second);
            return it->second;
        }
    }

    MultiWordEntry entry = trie.search(processedWord);
    if (!entry.word.empty()) {
        addToCache(processedWord, entry);
    }

    return entry;
}

std::string EnhancedDictionary::getMeaning(const std::string& word) {
    MultiWordEntry entry = getWordEntry(word);
    if (entry.word.empty() || entry.meanings.empty()) {
        return "Word not found in dictionary";
    }

    // Return all meanings concatenated
    std::string result;
    for (size_t i = 0; i < entry.meanings.size(); ++i) {
        if (i > 0) result += " | ";
        result += std::to_string(i + 1) + ". " + entry.meanings[i];
    }
    return result;
}

std::string EnhancedDictionary::getArabicTranslation(const std::string& word) {
    MultiWordEntry entry = getWordEntry(word);
    if (!entry.word.empty() && !entry.arabicTranslations.empty()) {
        // Return all Arabic translations
        std::string result;
        for (size_t i = 0; i < entry.arabicTranslations.size(); ++i) {
            if (!entry.arabicTranslations[i].empty()) {
                if (!result.empty()) result += " | ";
                result += std::to_string(i + 1) + ". " + entry.arabicTranslations[i];
            }
        }
        if (!result.empty()) return result;
    }

    return "Translation not available";
}

std::vector<std::string> EnhancedDictionary::searchByPrefix(const std::string& prefix, int maxResults) {
    if (prefix.empty()) return {};
    std::string processedPrefix = processWord(prefix);
    return trie.getWordSuggestions(processedPrefix, maxResults);
}

std::vector<MultiWordEntry> EnhancedDictionary::searchByMeaning(const std::string& meaningQuery,
    int maxResults) {
    std::vector<MultiWordEntry> results;
    if (meaningQuery.empty()) return results;

    std::string lowerQuery = processWord(meaningQuery);
    std::lock_guard<std::mutex> lock(dataMutex);

    for (const auto& entry : wordList) {
        // Search in all meanings
        for (const auto& meaning : entry.meanings) {
            std::string lowerMeaning = processWord(meaning);
            if (lowerMeaning.find(lowerQuery) != std::string::npos) {
                results.push_back(entry);
                break; // Only add the entry once
            }
        }
    }

    std::sort(results.begin(), results.end(),
        [&lowerQuery, this](const MultiWordEntry& a, const MultiWordEntry& b) {
            // Find best match position
            size_t posA = std::string::npos;
            size_t posB = std::string::npos;

            for (const auto& meaning : a.meanings) {
                std::string lowerMeaning = processWord(meaning);
                size_t pos = lowerMeaning.find(lowerQuery);
                if (pos < posA) posA = pos;
            }

            for (const auto& meaning : b.meanings) {
                std::string lowerMeaning = processWord(meaning);
                size_t pos = lowerMeaning.find(lowerQuery);
                if (pos < posB) posB = pos;
            }

            if (posA != posB) {
                return posA < posB;
            }

            return a.frequency > b.frequency;
        });

    if (static_cast<int>(results.size()) > maxResults) {
        results.resize(maxResults);
    }

    return results;
}

void EnhancedDictionary::loadLargeDictionary() {
    // Already called in constructor
}

void EnhancedDictionary::generateMassiveWordDatabase() {
    const std::vector<std::tuple<std::string, std::string, std::string, std::string>>
    wordDatabase = {
        // ANIMALS
        {"cat", "Small domesticated carnivorous mammal", "[translate:القطة حيوان أليف صغير آكل اللحوم]", "noun"},
        {"dog", "Domesticated carnivorous mammal", "[translate:الكلب حيوان أليف مفيد ووفي للإنسان]", "noun"},
        {"bird", "Feathered flying animal", "[translate:الطائر حيوان له ريش ويستطيع الطيران]", "noun"},
        {"fish", "Aquatic vertebrate animal", "[translate:السمك حيوان مائي يعيش في البحار والأنهار]", "noun"},
        {"horse", "Large domesticated mammal", "[translate:الحصان حيوان كبير يستخدم في الركوب والحمل]", "noun"},
        {"cow", "Large domesticated ungulate", "[translate:البقرة حيوان كبير ينتج الحليب واللحم]", "noun"},
        {"sheep", "Domesticated ruminant mammal", "[translate:الخروف حيوان أليف ينتج الصوف واللحم]", "noun"},
        {"chicken", "Domesticated fowl", "[translate:الدجاج طائر أليف ينتج البيض واللحم]", "noun"},
        {"rabbit", "Small mammal with long ears", "[translate:الأرنب حيوان صغير له أذنان طويلتان]", "noun"},
        {"elephant", "Large mammal with trunk", "[translate:الفيل أكبر الحيوانات البرية له خرطوم طويل]", "noun"},
        {"lion", "Large wild cat", "[translate:الأسد ملك الحيوانات وأقوى السنوريات]", "noun"},
        {"tiger", "Large striped wild cat", "[translate:النمر حيوان مفترس له خطوط جميلة على جسمه]", "noun"},

        // COMMON OBJECTS
        {"car", "Motor vehicle with four wheels", "[translate:السيارة مركبة بأربع عجلات تنقل الناس]", "noun"},
        {"house", "Building for human habitation", "[translate:البيت مكان يسكن فيه الإنسان مع أسرته]", "noun"},
        {"book", "Written or printed work", "[translate:الكتاب مجموعة أوراق مكتوبة تحتوي على المعلومات]", "noun"},
        {"table", "Piece of furniture with flat top", "[translate:الطاولة قطعة أثاث لوضع الأشياء عليها]", "noun"},
        {"chair", "Seat for one person", "[translate:الكرسي مقعد لشخص واحد للجلوس عليه]", "noun"},
        {"door", "Hinged barrier for entrance", "[translate:الباب مدخل البيت أو الغرفة للدخول والخروج]", "noun"},
        {"window", "Opening in wall for light", "[translate:النافذة فتحة في الجدار لدخول الضوء والهواء]", "noun"},
        {"phone", "Telecommunications device", "[translate:الهاتف جهاز للتواصل مع الآخرين عن بعد]", "noun"},
        {"computer", "Electronic device for processing data", "[translate:الحاسوب جهاز إلكتروني لمعالجة البيانات والمعلومات]", "noun"},
        {"television", "System for transmitting visual images", "[translate:التلفزيون جهاز لمشاهدة البرامج والأفلام]", "noun"},

        // VERBS
        {"go", "Move from one place to another", "[translate:يذهب من مكان إلى مكان آخر للوصول هناك]", "verb"},
        {"come", "Move toward or arrive at", "[translate:يأتي ويقترب من المكان المطلوب]", "verb"},
        {"walk", "Move on foot at regular pace", "[translate:يمشي على الأقدام بخطوات منتظمة]", "verb"},
        {"run", "Move on foot at fast pace", "[translate:يجري بسرعة على الأقدام للوصول بسرعة]", "verb"},
        {"eat", "Put food into mouth", "[translate:يأكل الطعام بفمه للحصول على الطاقة]", "verb"},
        {"drink", "Take liquid into mouth", "[translate:يشرب السوائل لإرواء عطشه وترطيب جسمه]", "verb"},
        {"read", "Look at words and understand", "[translate:يقرأ الكلمات والنصوص لفهم المعلومات]", "verb"},
        {"write", "Put words on paper", "[translate:يكتب الكلمات والأفكار على الورق]", "verb"},
        {"speak", "Say words aloud", "[translate:يتكلم ويقول الكلمات بصوت مسموع]", "verb"},
        {"listen", "Pay attention to sound", "[translate:يستمع بإنتباه إلى الأصوات والكلام]", "verb"},

        // ADJECTIVES
        {"big", "Of considerable size", "[translate:كبير الحجم والمساحة أكثر من الحد العادي]", "adjective"},
        {"small", "Of little size", "[translate:صغير الحجم والمقاس أقل من المعتاد]", "adjective"},
        {"good", "To be desired", "[translate:جيد ومرغوب فيه ومفيد للاستخدام]", "adjective"},
        {"bad", "Of poor quality", "[translate:سيء وغير مرغوب فيه وضار]", "adjective"},
        {"new", "Not existing before", "[translate:جديد ولم يوجد من قبل أو حديث الصنع]", "adjective"},
        {"old", "Having existed for long time", "[translate:قديم وموجود منذ زمن طويل]", "adjective"},
        {"hot", "Having high temperature", "[translate:حار ودرجة حرارته عالية ومرتفعة]", "adjective"},
        {"cold", "Having low temperature", "[translate:بارد ودرجة حرارته منخفضة ومنعشة]", "adjective"},

        // COLORS
        {"red", "Color of blood", "[translate:أحمر مثل لون الدم والورود الجميلة]", "adjective"},
        {"blue", "Color of clear sky", "[translate:أزرق مثل لون السماء الصافية والبحر]", "adjective"},
        {"green", "Color of grass", "[translate:أخضر مثل لون العشب والأشجار في الطبيعة]", "adjective"},
        {"yellow", "Color of sun", "[translate:أصفر مثل لون الشمس المشرقة والذهب]", "adjective"},
        {"black", "Dark color", "[translate:أسود وهو لون الظلام والليل المظلم]", "adjective"},
        {"white", "Light color", "[translate:أبيض وهو لون النقاء والثلج الأبيض]", "adjective"},

        // FAMILY
        {"family", "Group of related people", "[translate:الأسرة مجموعة من الأقارب يعيشون معاً]", "noun"},
        {"father", "Male parent", "[translate:الأب هو الرجل الذي ينجب الأطفال ويربيهم]", "noun"},
        {"mother", "Female parent", "[translate:الأم هي المرأة التي تنجب الأطفال وتعتني بهم]", "noun"},
        {"brother", "Male sibling", "[translate:الأخ هو الذكر الذي له نفس الوالدين]", "noun"},
        {"sister", "Female sibling", "[translate:الأخت هي الأنثى التي لها نفس الوالدين]", "noun"},

        // COMMON WORDS
        {"hello", "Greeting word", "[translate:مرحبا كلمة للترحيب والسلام على الناس]", "interjection"},
        {"goodbye", "Farewell word", "[translate:وداعا كلمة للفراق والسلام عند الرحيل]", "interjection"},
        {"yes", "Affirmative response", "[translate:نعم كلمة للموافقة والإجابة الإيجابية]", "interjection"},
        {"no", "Negative response", "[translate:لا كلمة للرفض والإجابة السلبية]", "interjection"},
        {"water", "Clear liquid", "[translate:الماء سائل شفاف ضروري للحياة والشرب]", "noun"},
        {"food", "Substance for nutrition", "[translate:الطعام مواد يأكلها الإنسان للحصول على الطاقة]", "noun"},
        {"time", "Concept of duration", "[translate:الوقت مفهوم يقيس مرور الأحداث والساعات]", "noun"},
        {"world", "Planet Earth", "[translate:العالم كوكب الأرض وجميع الناس والأماكن]", "noun"},
        {"home", "Place of residence", "[translate:البيت مكان السكن والراحة مع الأهل]", "noun"},
        {"friend", "Person you like", "[translate:الصديق شخص تحبه ويساعدك في الحياة]", "noun"},
        {"money", "Medium of exchange", "[translate:المال وسيلة لشراء الأشياء والحصول عليها]", "noun"},
        {"life", "State of being alive", "[translate:الحياة حالة الوجود والعيش في العالم]", "noun"}
    };

    for (const auto& [word, meaning, arabic, pos] : wordDatabase) {
        MultiWordEntry entry(word, meaning, arabic, pos);
        entry.frequency = 5; // Built-in words have higher frequency

        // Don't save built-in words to user file, only add to memory
        std::lock_guard<std::mutex> lock(dataMutex);
        std::string processedWord = processWord(entry.word);

        trie.insert(entry);
        hashTable[processedWord] = entry;
        wordList.push_back(entry);
    }
}

int EnhancedDictionary::getWordCount() const {
    std::lock_guard<std::mutex> lock(dataMutex);
    return wordList.size();
}

// NEW: Persistent storage methods
bool EnhancedDictionary::saveUserWords() const {
    // Save as UTF-8 even on Windows
    std::ofstream file(userWordsFile, std::ios::binary); // use binary for full UTF-8 compatibility
    if (!file.is_open()) {
        return false;
    }

    // Optional: Write UTF-8 BOM
    file << "\xEF\xBB\xBF";

    // Save only user-added words (not built-in ones)
    for (const auto& entry : wordList) {
        if (entry.frequency == 1) {
            file << multiWordEntryToString(entry) << std::endl;
        }
    }
    return true;
}

// For loading, check for BOM and use imbue, or just rely on default on modern systems:
bool EnhancedDictionary::loadUserWords() {
    std::ifstream file(userWordsFile, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    // Skip BOM if present
    char bom[3] = {0};
    file.read(bom, 3);
    if (!(bom[0] == static_cast<char>(0xEF) && bom[1] == static_cast<char>(0xBB) && bom[2] == static_cast<char>(0xBF))) {
        // No BOM, seek to start
        file.seekg(0);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            MultiWordEntry entry = stringToMultiWordEntry(line);
            if (!entry.word.empty()) {
                std::lock_guard<std::mutex> lock(dataMutex);
                std::string processedWord = processWord(entry.word);
                trie.insert(entry);
                hashTable[processedWord] = entry;
                wordList.push_back(entry);
            }
        }
    }
    return true;
}

// Helper methods for file operations
std::string EnhancedDictionary::multiWordEntryToString(const MultiWordEntry& entry) const {
    std::ostringstream oss;
    oss << entry.word << "|" << entry.frequency;

    // Store all meanings
    for (size_t i = 0; i < entry.meanings.size(); ++i) {
        oss << "|" << entry.meanings[i];
        oss << "|" << (i < entry.arabicTranslations.size() ? entry.arabicTranslations[i] : "");
        oss << "|" << (i < entry.partOfSpeeches.size() ? entry.partOfSpeeches[i] : "");
    }

    return oss.str();
}

MultiWordEntry EnhancedDictionary::stringToMultiWordEntry(const std::string& line) const {
    std::istringstream iss(line);
    std::string word, freqStr;

    if (!std::getline(iss, word, '|') || !std::getline(iss, freqStr, '|')) {
        return MultiWordEntry();
    }

    int frequency = 1;
    try {
        frequency = std::stoi(freqStr);
    } catch (...) {
        frequency = 1;
    }

    MultiWordEntry entry;
    entry.word = word;
    entry.frequency = frequency;

    // Read all meanings (groups of 3: meaning, arabic, pos)
    std::string meaning, arabic, pos;
    while (std::getline(iss, meaning, '|')) {
        std::getline(iss, arabic, '|');
        std::getline(iss, pos, '|');
        entry.addMeaning(meaning, arabic, pos);
    }

    return entry;
}

std::string EnhancedDictionary::getArabicInputSafe(const std::string& prompt) const {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

void EnhancedDictionary::addToCache(const std::string& key, const MultiWordEntry& value) const {
    if (searchCache.size() >= MAX_CACHE_SIZE) {
        searchCache.clear();
    }

    searchCache[key] = value;
}

void EnhancedDictionary::printDetailedStatistics() const {
    std::cout << "Total words in dictionary: " << getWordCount() << std::endl;
    std::cout << "Hash table entries: " << hashTable.size() << std::endl;

    if (totalSearches > 0) {
        std::cout << "Total searches performed: " << totalSearches << std::endl;
        std::cout << "Cache hit ratio: " << std::fixed << std::setprecision(2)
            << (static_cast<double>(cacheHits) / totalSearches) * 100 << "%" << std::endl;
    }

    std::cout << "User words file: " << userWordsFile << std::endl;
}

bool EnhancedDictionary::validateWordEntry(const MultiWordEntry& entry) const {
    return !entry.word.empty() && !entry.meanings.empty() &&
        entry.word.length() <= 100 && entry.meanings[0].length() <= 500;
}