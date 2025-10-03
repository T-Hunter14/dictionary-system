#ifndef ENHANCED_DICTIONARY_H
#define ENHANCED_DICTIONARY_H

#include "advanced_trie.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

struct Meaning {
    std::string definition;
    std::string arabic;
    Meaning() {}
    Meaning(const std::string& def, const std::string& ar) : definition(def), arabic(ar) {}
};

struct Word {
    std::string word;
    std::vector<Meaning> meanings;
    Word() {}
    Word(const std::string& w) : word(w) {}
};

class Dictionary {
private:
    Trie trie;
    std::unordered_map<std::string, Word> words;
    std::unordered_map<std::string, std::vector<std::string>> arabicIndex;
public:
    Dictionary();
    bool addWord(const std::string& english, const std::string& meaning, const std::string& arabic = "");
    bool addMeaning(const std::string& english, const std::string& meaning, const std::string& arabic = "");
    bool deleteMeaning(const std::string& english, int meaningIndex = -1);
    Word* findWord(const std::string& english);
    std::vector<std::string> findEnglish(const std::string& arabic);
    std::vector<std::string> autocomplete(const std::string& prefix);
    int wordCount() const { return words.size(); }
    int meaningCount() const;
private:
    void loadBuiltIn();
    void save();
    void load();
    std::string toLower(const std::string& s) const;
    void updateArabicIndex(const std::string& arabic, const std::string& english);
    void removeFromArabicIndex(const std::string& arabic, const std::string& english);
};

#endif
