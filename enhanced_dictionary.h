#ifndef ENHANCED_DICTIONARY_H
#define ENHANCED_DICTIONARY_H

#include "advanced_trie.h"
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <iostream>

struct Meaning {
    std::string definition;
    std::string arabic;
    Meaning() = default;
    Meaning(std::string def, std::string ar)
        : definition(std::move(def)), arabic(std::move(ar)) {}
};

struct Word {
    std::string word;
    std::vector<Meaning> meanings;
    Word() = default;
    explicit Word(std::string w) : word(std::move(w)) {}
};

class Dictionary {
public:
    Dictionary();
    ~Dictionary();

    // Returns false when the word already exists.
    bool addWord(std::string_view english, std::string_view meaning,
                 std::string_view arabic = "");

    // Returns false when the word does not exist.
    bool addMeaning(std::string_view english, std::string_view meaning,
                    std::string_view arabic = "");

    // meaningIndex == -1  → delete entire word.
    bool deleteMeaning(std::string_view english, int meaningIndex = -1);

    Word* findWord(std::string_view english);

    // Arabic → list of English words.
    std::vector<std::string> findEnglish(std::string_view arabic) const;

    std::vector<std::string> autocomplete(std::string_view prefix) const;

    int wordCount()    const noexcept { return static_cast<int>(words_.size()); }
    int meaningCount() const noexcept { return totalMeanings_; }  // O(1)

private:
    Trie trie_;
    std::unordered_map<std::string, Word> words_;

    // Arabic → set of English words (O(1) duplicate check)
    std::unordered_map<std::string, std::unordered_set<std::string>> arabicIndex_;

    int  totalMeanings_ = 0;   // cached count, updated on every mutation
    bool dirty_         = false;

    void loadBuiltIn();
    void save();
    void load();

    // Modify string in place to all-lowercase; no heap allocation.
    static void toLowerInPlace(std::string& s) noexcept;

    // Build a lowercase lookup key from a string_view.
    static std::string makeKey(std::string_view s);

    void updateArabicIndex(std::string_view arabic, std::string_view english);
    void removeFromArabicIndex(std::string_view arabic, std::string_view english);
};

#endif
