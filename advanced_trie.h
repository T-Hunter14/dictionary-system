#ifndef ADVANCED_TRIE_H
#define ADVANCED_TRIE_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// Forward declaration from enhanced_dictionary.h
struct MultiWordEntry;

struct TrieNode {
    std::unordered_map<char, std::unique_ptr<TrieNode>> children;
    bool isEndOfWord;
    MultiWordEntry* wordData; // Store pointer to avoid circular dependency

    TrieNode() : isEndOfWord(false), wordData(nullptr) {}
    ~TrieNode();
};

class AdvancedTrie {
private:
    std::unique_ptr<TrieNode> root;
    int totalWords;

    void collectWords(TrieNode* node, const std::string& prefix,
        std::vector<MultiWordEntry>& results, int maxResults) const;
    size_t calculateMemoryUsage(TrieNode* node) const;

public:
    AdvancedTrie();
    bool insert(const MultiWordEntry& entry);
    MultiWordEntry search(const std::string& word) const;
    bool contains(const std::string& word) const;
    std::vector<std::string> getWordSuggestions(const std::string& prefix, int maxResults = 20) const;
    std::vector<MultiWordEntry> getAutoCompleteWords(const std::string& prefix, int maxResults = 20) const;
    void clear();
    int getWordCount() const { return totalWords; }
    bool empty() const { return totalWords == 0; }
    size_t getMemoryUsage() const;
    void optimizeMemory();
};

#endif // ADVANCED_TRIE_H
