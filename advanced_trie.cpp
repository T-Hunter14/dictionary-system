#include "advanced_trie.h"

Trie::Trie() {
    root = std::make_unique<TrieNode>();
}

void Trie::insert(const std::string& word) {
    if (word.empty()) return;

    std::string lower = word;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    TrieNode* node = root.get();
    for (char ch : lower) {
        if (node->children.find(ch) == node->children.end()) {
            node->children[ch] = std::make_unique<TrieNode>();
        }
        node = node->children[ch].get();
    }

    node->isWord = true;
    node->originalWord = word;
}

bool Trie::contains(const std::string& word) {
    if (word.empty()) return false;

    std::string lower = word;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    TrieNode* node = root.get();
    for (char ch : lower) {
        if (node->children.find(ch) == node->children.end()) {
            return false;
        }
        node = node->children[ch].get();
    }

    return node->isWord;
}

std::vector<std::string> Trie::autocomplete(const std::string& prefix, int maxResults) {
    std::vector<std::string> results;
    if (prefix.empty()) return results;

    std::string lower = prefix;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    TrieNode* node = root.get();
    for (char ch : lower) {
        if (node->children.find(ch) == node->children.end()) {
            return results;
        }
        node = node->children[ch].get();
    }

    collectWords(node, results, maxResults);
    return results;
}

void Trie::collectWords(TrieNode* node, std::vector<std::string>& results, int maxResults) {
    if (!node || static_cast<int>(results.size()) >= maxResults) {
        return;
    }

    if (node->isWord) {
        results.push_back(node->originalWord);
        if (static_cast<int>(results.size()) >= maxResults) {
            return;
        }
    }

    for (auto& pair : node->children) {
        collectWords(pair.second.get(), results, maxResults);
        if (static_cast<int>(results.size()) >= maxResults) {
            return;
        }
    }
}

bool Trie::remove(const std::string& word) {
    if (word.empty()) return false;

    std::string lower = word;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    TrieNode* node = root.get();
    for (char ch : lower) {
        if (node->children.find(ch) == node->children.end()) {
            return false;
        }
        node = node->children[ch].get();
    }

    if (node->isWord) {
        node->isWord = false;
        node->originalWord.clear();
        return true;
    }

    return false;
}
