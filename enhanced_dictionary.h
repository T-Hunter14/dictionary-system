#ifndef ENHANCED_DICTIONARY_H
#define ENHANCED_DICTIONARY_H

#include "advanced_trie.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <mutex>
#include <fstream>

// Enhanced WordEntry to support multiple meanings
struct MultiWordEntry {
    std::string word;
    std::vector<std::string> meanings;
    std::vector<std::string> arabicTranslations;
    std::vector<std::string> partOfSpeeches;
    int frequency;

    MultiWordEntry() : frequency(1) {}

    MultiWordEntry(const std::string& w, const std::string& m,
                   const std::string& arabic = "", const std::string& pos = "")
        : word(w), frequency(1) {
        meanings.push_back(m);
        arabicTranslations.push_back(arabic);
        partOfSpeeches.push_back(pos);
    }

    void addMeaning(const std::string& meaning, const std::string& arabic = "",
                    const std::string& pos = "") {
        meanings.push_back(meaning);
        arabicTranslations.push_back(arabic);
        partOfSpeeches.push_back(pos);
    }

    bool removeMeaning(int index) {
        if (index >= 0 && index < static_cast<int>(meanings.size())) {
            meanings.erase(meanings.begin() + index);
            arabicTranslations.erase(arabicTranslations.begin() + index);
            partOfSpeeches.erase(partOfSpeeches.begin() + index);
            return true;
        }
        return false;
    }
};

class EnhancedDictionary {
private:
    AdvancedTrie trie;
    std::unordered_map<std::string, MultiWordEntry> hashTable;
    std::vector<MultiWordEntry> wordList;

    mutable std::unordered_map<std::string, MultiWordEntry> searchCache;
    static const size_t MAX_CACHE_SIZE = 500;

    mutable std::mutex dataMutex;
    mutable size_t totalSearches;
    mutable size_t cacheHits;

    // File storage for persistent words
    std::string userWordsFile;

public:
    EnhancedDictionary();

    bool addWord(const MultiWordEntry& entry);
    bool addWord(const std::string& word, const std::string& meaning,
                 const std::string& arabic = "", const std::string& pos = "");

    // Enhanced: Add meaning to existing word
    bool addMeaningToWord(const std::string& word, const std::string& meaning,
                         const std::string& arabic = "", const std::string& pos = "");

    // Enhanced: Delete specific meaning or entire word
    bool deleteWordMeaning(const std::string& word, int meaningIndex = -1);
    std::vector<std::string> getWordMeanings(const std::string& word);

    MultiWordEntry getWordEntry(const std::string& word);
    std::string getMeaning(const std::string& word);
    std::string getArabicTranslation(const std::string& word);

    std::vector<std::string> searchByPrefix(const std::string& prefix, int maxResults = 20);
    std::vector<MultiWordEntry> searchByMeaning(const std::string& meaningQuery, int maxResults = 10);

    void loadLargeDictionary();
    void printDetailedStatistics() const;

    int getWordCount() const;

    // Persistent storage methods
    bool saveUserWords() const;
    bool loadUserWords();

    // Arabic input helper
    std::string getArabicInputSafe(const std::string& prompt) const;

private:
    std::string processWord(const std::string& word) const;
    void addToCache(const std::string& key, const MultiWordEntry& value) const;
    void generateMassiveWordDatabase();
    bool validateWordEntry(const MultiWordEntry& entry) const;

    // Helper methods for file operations
    std::string multiWordEntryToString(const MultiWordEntry& entry) const;
    MultiWordEntry stringToMultiWordEntry(const std::string& line) const;
};

#endif // ENHANCED_DICTIONARY_H