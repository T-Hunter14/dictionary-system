#ifndef ADVANCED_TRIE_H
#define ADVANCED_TRIE_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>

struct TrieNode {
    std::unordered_map<char, std::unique_ptr<TrieNode>> children;
    bool isWord;
    std::string originalWord;
    TrieNode() : isWord(false) {}
};

class Trie {
private:
    std::unique_ptr<TrieNode> root;
    void collectWords(TrieNode* node, std::vector<std::string>& results, int maxResults);
public:
    Trie();
    void insert(const std::string& word);
    bool contains(const std::string& word);
    std::vector<std::string> autocomplete(const std::string& prefix, int maxResults = 20);
    bool remove(const std::string& word);
};

#endif
