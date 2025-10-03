#include "enhanced_dictionary.h"

Dictionary::Dictionary() {
    loadBuiltIn();
    load();
}

std::string Dictionary::toLower(const std::string& s) const {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

bool Dictionary::addWord(const std::string& english, const std::string& meaning, const std::string& arabic) {
    if (english.empty() || meaning.empty()) return false;
    std::string key = toLower(english);
    if (words.find(key) != words.end()) return false;
    Word newWord(english);
    newWord.meanings.push_back(Meaning(meaning, arabic));
    words[key] = newWord;
    trie.insert(english);
    if (!arabic.empty()) updateArabicIndex(arabic, english);
    save();
    return true;
}

bool Dictionary::addMeaning(const std::string& english, const std::string& meaning, const std::string& arabic) {
    if (english.empty() || meaning.empty()) return false;
    std::string key = toLower(english);
    auto it = words.find(key);
    if (it == words.end()) return false;
    it->second.meanings.push_back(Meaning(meaning, arabic));
    if (!arabic.empty()) updateArabicIndex(arabic, english);
    save();
    return true;
}

bool Dictionary::deleteMeaning(const std::string& english, int meaningIndex) {
    if (english.empty()) return false;
    std::string key = toLower(english);
    auto it = words.find(key);
    if (it == words.end()) return false;
    Word& word = it->second;
    if (meaningIndex == -1) {
        for (const Meaning& m : word.meanings) {
            if (!m.arabic.empty()) removeFromArabicIndex(m.arabic, english);
        }
        trie.remove(english);
        words.erase(it);
        save();
        return true;
    }
    if (meaningIndex >= 0 && meaningIndex < static_cast<int>(word.meanings.size())) {
        const Meaning& m = word.meanings[meaningIndex];
        if (!m.arabic.empty()) removeFromArabicIndex(m.arabic, english);
        word.meanings.erase(word.meanings.begin() + meaningIndex);
        if (word.meanings.empty()) {
            trie.remove(english);
            words.erase(it);
        }
        save();
        return true;
    }
    return false;
}

Word* Dictionary::findWord(const std::string& english) {
    std::string key = toLower(english);
    auto it = words.find(key);
    return (it != words.end()) ? &it->second : nullptr;
}

std::vector<std::string> Dictionary::findEnglish(const std::string& arabic) {
    auto it = arabicIndex.find(arabic);
    return (it != arabicIndex.end()) ? it->second : std::vector<std::string>();
}

std::vector<std::string> Dictionary::autocomplete(const std::string& prefix) {
    return trie.autocomplete(prefix, 20);
}

int Dictionary::meaningCount() const {
    int count = 0;
    for (const auto& pair : words) count += pair.second.meanings.size();
    return count;
}

void Dictionary::updateArabicIndex(const std::string& arabic, const std::string& english) {
    if (arabic.empty()) return;
    auto& vec = arabicIndex[arabic];
    if (std::find(vec.begin(), vec.end(), english) == vec.end()) vec.push_back(english);
}

void Dictionary::removeFromArabicIndex(const std::string& arabic, const std::string& english) {
    if (arabic.empty()) return;
    auto it = arabicIndex.find(arabic);
    if (it != arabicIndex.end()) {
        auto& vec = it->second;
        vec.erase(std::remove(vec.begin(), vec.end(), english), vec.end());
        if (vec.empty()) arabicIndex.erase(it);
    }
}

void Dictionary::loadBuiltIn() {
    struct BuiltIn { const char* word; const char* meaning; const char* arabic; };
    BuiltIn data[] = {
        {"hello", "Greeting", "مرحبا"},
        {"book", "Written work", "كتاب"},
        {"cat", "Small mammal", "قطة"},
        {"dog", "Animal", "كلب"},
        {"water", "Liquid", "ماء"},
        {"food", "Edible", "طعام"},
        {"house", "Building", "بيت"},
        {"car", "Vehicle", "سيارة"},
        {"friend", "Person", "صديق"},
        {"time", "Duration", "وقت"}
    };
    for (const auto& item : data) {
        std::string key = toLower(item.word);
        Word w(item.word);
        w.meanings.push_back(Meaning(item.meaning, item.arabic));
        words[key] = w;
        trie.insert(item.word);
        updateArabicIndex(item.arabic, item.word);
    }
}

void Dictionary::save() {
    std::ofstream file("dictionary.txt", std::ios::binary);
    if (!file.is_open()) return;
    file.write("\xEF\xBB\xBF", 3);
    for (const auto& pair : words) {
        const Word& w = pair.second;
        file << w.word;
        for (const Meaning& m : w.meanings) {
            file << "|" << m.definition << "|" << m.arabic;
        }
        file << "\n";
    }
}

void Dictionary::load() {
    std::ifstream file("dictionary.txt", std::ios::binary);
    if (!file.is_open()) return;
    char bom[3];
    file.read(bom, 3);
    if (!(bom[0] == (char)0xEF && bom[1] == (char)0xBB && bom[2] == (char)0xBF)) {
        file.seekg(0);
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string wordStr;
        if (!std::getline(iss, wordStr, '|')) continue;
        std::string key = toLower(wordStr);
        Word w(wordStr);
        std::string meaning, arabic;
        while (std::getline(iss, meaning, '|')) {
            std::getline(iss, arabic, '|');
            w.meanings.push_back(Meaning(meaning, arabic));
            if (!arabic.empty()) updateArabicIndex(arabic, wordStr);
        }
        words[key] = w;
        trie.insert(wordStr);
    }
}
