#include "enhanced_dictionary.h"
#include <cctype>
#include <cstring>

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

void Dictionary::toLowerInPlace(std::string& s) noexcept {
    for (char& c : s)
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
}

std::string Dictionary::makeKey(std::string_view s) {
    std::string key(s);
    toLowerInPlace(key);
    return key;
}

// ---------------------------------------------------------------------------
// Constructor / Destructor
// ---------------------------------------------------------------------------

Dictionary::Dictionary() {
    words_.reserve(512);
    loadBuiltIn();
    load();
}

// Flush any unsaved changes when the object is destroyed.
Dictionary::~Dictionary() {
    if (dirty_) save();
}

// ---------------------------------------------------------------------------
// addWord
// ---------------------------------------------------------------------------
bool Dictionary::addWord(std::string_view english, std::string_view meaning,
                          std::string_view arabic) {
    if (english.empty() || meaning.empty()) return false;

    std::string key = makeKey(english);
    // try_emplace does nothing if key already present — single lookup.
    auto [it, inserted] = words_.try_emplace(key, std::string(english));
    if (!inserted) return false;

    it->second.meanings.emplace_back(std::string(meaning), std::string(arabic));
    ++totalMeanings_;

    trie_.insert(english);
    if (!arabic.empty()) updateArabicIndex(arabic, english);

    dirty_ = true;
    save();
    return true;
}

// ---------------------------------------------------------------------------
// addMeaning
// ---------------------------------------------------------------------------
bool Dictionary::addMeaning(std::string_view english, std::string_view meaning,
                              std::string_view arabic) {
    if (english.empty() || meaning.empty()) return false;

    std::string key = makeKey(english);
    auto it = words_.find(key);
    if (it == words_.end()) return false;

    it->second.meanings.emplace_back(std::string(meaning), std::string(arabic));
    ++totalMeanings_;

    if (!arabic.empty()) updateArabicIndex(arabic, english);

    dirty_ = true;
    save();
    return true;
}

// ---------------------------------------------------------------------------
// deleteMeaning
// ---------------------------------------------------------------------------
bool Dictionary::deleteMeaning(std::string_view english, int meaningIndex) {
    if (english.empty()) return false;

    std::string key = makeKey(english);
    auto it = words_.find(key);
    if (it == words_.end()) return false;

    Word& word = it->second;

    if (meaningIndex == -1) {
        // Delete entire word
        for (const Meaning& m : word.meanings) {
            if (!m.arabic.empty()) removeFromArabicIndex(m.arabic, english);
        }
        totalMeanings_ -= static_cast<int>(word.meanings.size());
        trie_.remove(english);
        words_.erase(it);
        dirty_ = true;
        save();
        return true;
    }

    if (meaningIndex >= 0 && meaningIndex < static_cast<int>(word.meanings.size())) {
        const Meaning& m = word.meanings[meaningIndex];
        if (!m.arabic.empty()) removeFromArabicIndex(m.arabic, english);
        word.meanings.erase(word.meanings.begin() + meaningIndex);
        --totalMeanings_;

        if (word.meanings.empty()) {
            trie_.remove(english);
            words_.erase(it);
        }
        dirty_ = true;
        save();
        return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
// findWord
// ---------------------------------------------------------------------------
Word* Dictionary::findWord(std::string_view english) {
    std::string key = makeKey(english);
    auto it = words_.find(key);
    return (it != words_.end()) ? &it->second : nullptr;
}

// ---------------------------------------------------------------------------
// findEnglish (Arabic → English)
// ---------------------------------------------------------------------------
std::vector<std::string> Dictionary::findEnglish(std::string_view arabic) const {
    std::string key(arabic);
    auto it = arabicIndex_.find(key);
    if (it == arabicIndex_.end()) return {};
    return {it->second.begin(), it->second.end()};
}

// ---------------------------------------------------------------------------
// autocomplete
// ---------------------------------------------------------------------------
std::vector<std::string> Dictionary::autocomplete(std::string_view prefix) const {
    return trie_.autocomplete(prefix, 20);
}

// ---------------------------------------------------------------------------
// updateArabicIndex / removeFromArabicIndex
// ---------------------------------------------------------------------------
void Dictionary::updateArabicIndex(std::string_view arabic, std::string_view english) {
    if (arabic.empty()) return;
    arabicIndex_.try_emplace(std::string(arabic)).first->second.emplace(english);
}

void Dictionary::removeFromArabicIndex(std::string_view arabic,
                                        std::string_view english) {
    if (arabic.empty()) return;
    std::string arKey(arabic);
    auto it = arabicIndex_.find(arKey);
    if (it != arabicIndex_.end()) {
        std::string enKey(english);
        it->second.erase(enKey);
        if (it->second.empty()) arabicIndex_.erase(it);
    }
}

// ---------------------------------------------------------------------------
// loadBuiltIn
// ---------------------------------------------------------------------------
void Dictionary::loadBuiltIn() {
    static const struct { const char* word; const char* meaning; const char* arabic; }
    kBuiltIn[] = {
        {"hello",  "Greeting",  "مرحبا"},
        {"book",   "Written work", "كتاب"},
        {"cat",    "Small mammal", "قطة"},
        {"dog",    "Animal",    "كلب"},
        {"water",  "Liquid",    "ماء"},
        {"food",   "Edible",    "طعام"},
        {"house",  "Building",  "بيت"},
        {"car",    "Vehicle",   "سيارة"},
        {"friend", "Person",    "صديق"},
        {"time",   "Duration",  "وقت"}
    };

    for (const auto& item : kBuiltIn) {
        std::string key = makeKey(item.word);
        auto [it, inserted] = words_.try_emplace(key, std::string(item.word));
        if (inserted) {
            it->second.meanings.emplace_back(std::string(item.meaning),
                                              std::string(item.arabic));
            ++totalMeanings_;
            trie_.insert(item.word);
            updateArabicIndex(item.arabic, item.word);
        }
    }
}

// ---------------------------------------------------------------------------
// save  — build entire content into a buffer, one write call
// ---------------------------------------------------------------------------
void Dictionary::save() {
    std::string buf;
    buf.reserve(4096);
    // UTF-8 BOM
    buf += "\xEF\xBB\xBF";
    for (const auto& [key, w] : words_) {
        buf += w.word;
        for (const Meaning& m : w.meanings) {
            buf += '|';
            buf += m.definition;
            buf += '|';
            buf += m.arabic;
        }
        buf += '\n';
    }
    std::ofstream file("dictionary.txt", std::ios::binary | std::ios::trunc);
    if (file.is_open()) {
        file.write(buf.data(), static_cast<std::streamsize>(buf.size()));
    }
    dirty_ = false;
}

// ---------------------------------------------------------------------------
// load  — read whole file into memory, parse without istringstream
// ---------------------------------------------------------------------------
void Dictionary::load() {
    std::ifstream file("dictionary.txt", std::ios::binary | std::ios::ate);
    if (!file.is_open()) return;

    auto size = file.tellg();
    if (size <= 0) return;

    std::string content(static_cast<std::size_t>(size), '\0');
    file.seekg(0);
    file.read(content.data(), size);

    const char* p   = content.data();
    const char* end = p + content.size();

    // Skip UTF-8 BOM if present
    if (content.size() >= 3 &&
        static_cast<unsigned char>(p[0]) == 0xEF &&
        static_cast<unsigned char>(p[1]) == 0xBB &&
        static_cast<unsigned char>(p[2]) == 0xBF)
        p += 3;

    while (p < end) {
        // Find end of line
        const char* lineEnd = p;
        while (lineEnd < end && *lineEnd != '\n') ++lineEnd;

        // Parse fields separated by '|'
        const char* fieldStart = p;
        const char* fieldEnd   = fieldStart;

        // First field: the word
        while (fieldEnd < lineEnd && *fieldEnd != '|') ++fieldEnd;
        if (fieldEnd == fieldStart) { p = lineEnd + 1; continue; }

        std::string wordStr(fieldStart, fieldEnd);
        // Strip trailing '\r' if present (CRLF files)
        if (!wordStr.empty() && wordStr.back() == '\r') wordStr.pop_back();
        if (wordStr.empty()) { p = lineEnd + 1; continue; }

        std::string key = makeKey(wordStr);

        // Skip words already loaded from built-in data (built-in takes priority)
        auto [it, inserted] = words_.try_emplace(key, wordStr);
        if (!inserted) { p = lineEnd + 1; continue; }

        // Remaining fields come in pairs: meaning|arabic
        const char* cur = fieldEnd;
        while (cur < lineEnd) {
            ++cur;  // skip '|'
            const char* mStart = cur;
            while (cur < lineEnd && *cur != '|') ++cur;
            std::string meaning(mStart, cur);

            std::string arabic;
            if (cur < lineEnd) {
                ++cur;  // skip '|'
                const char* aStart = cur;
                while (cur < lineEnd && *cur != '|') ++cur;
                arabic.assign(aStart, cur);
            }

            if (!meaning.empty()) {
                it->second.meanings.emplace_back(std::move(meaning), arabic);
                ++totalMeanings_;
                if (!arabic.empty()) updateArabicIndex(arabic, wordStr);
            }
        }

        trie_.insert(wordStr);
        p = lineEnd + 1;
    }
}
