#include "advanced_trie.h"
#include "enhanced_dictionary.h"
#include <algorithm>
#include <queue>

TrieNode::~TrieNode() {
    delete wordData;
}

AdvancedTrie::AdvancedTrie() : root(std::make_unique<TrieNode>()), totalWords(0) {}

std::string toLowerCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return result;
}

bool AdvancedTrie::insert(const MultiWordEntry& entry) {
    if (entry.word.empty()) return false;

    TrieNode* current = root.get();
    std::string lowerWord = toLowerCase(entry.word);

    for (char ch : lowerWord) {
        auto& childRef = current->children[ch];
        if (!childRef) {
            childRef = std::make_unique<TrieNode>();
        }
        current = childRef.get();
    }

    if (!current->isEndOfWord) {
        totalWords++;
        current->isEndOfWord = true;
    }

    // Store a copy of the entry
    delete current->wordData; // Clean up old data if exists
    current->wordData = new MultiWordEntry(entry);
    current->wordData->word = entry.word; // Preserve original case

    return true;
}

MultiWordEntry AdvancedTrie::search(const std::string& word) const {
    if (word.empty()) return MultiWordEntry();

    TrieNode* current = root.get();
    std::string lowerWord = toLowerCase(word);

    for (char ch : lowerWord) {
        auto it = current->children.find(ch);
        if (it == current->children.end()) {
            return MultiWordEntry();
        }
        current = it->second.get();
    }

    if (current->isEndOfWord && current->wordData) {
        return *current->wordData;
    }
    return MultiWordEntry();
}

bool AdvancedTrie::contains(const std::string& word) const {
    if (word.empty()) return false;

    TrieNode* current = root.get();
    std::string lowerWord = toLowerCase(word);

    for (char ch : lowerWord) {
        auto it = current->children.find(ch);
        if (it == current->children.end()) {
            return false;
        }
        current = it->second.get();
    }

    return current->isEndOfWord;
}

std::vector<MultiWordEntry> AdvancedTrie::getAutoCompleteWords(const std::string& prefix,
    int maxResults) const {
    std::vector<MultiWordEntry> results;
    if (prefix.empty() || maxResults <= 0) return results;

    TrieNode* current = root.get();
    std::string lowerPrefix = toLowerCase(prefix);

    // Navigate to prefix node
    for (char ch : lowerPrefix) {
        auto it = current->children.find(ch);
        if (it == current->children.end()) {
            return results;
        }
        current = it->second.get();
    }

    // Collect words from prefix node
    collectWords(current, prefix, results, maxResults);

    // Sort by frequency (highest first)
    std::sort(results.begin(), results.end(),
        [](const MultiWordEntry& a, const MultiWordEntry& b) {
            return a.frequency > b.frequency;
        });

    return results;
}

void AdvancedTrie::collectWords(TrieNode* node, const std::string& prefix,
    std::vector<MultiWordEntry>& results, int maxResults) const {
    if (!node || static_cast<int>(results.size()) >= maxResults) return;

    if (node->isEndOfWord && node->wordData) {
        results.push_back(*node->wordData);
        if (static_cast<int>(results.size()) >= maxResults) return;
    }

    for (const auto& pair : node->children) {
        collectWords(pair.second.get(), prefix + pair.first, results, maxResults);
        if (static_cast<int>(results.size()) >= maxResults) return;
    }
}

std::vector<std::string> AdvancedTrie::getWordSuggestions(const std::string& prefix,
    int maxResults) const {
    auto entries = getAutoCompleteWords(prefix, maxResults);
    std::vector<std::string> suggestions;
    suggestions.reserve(entries.size());

    for (const auto& entry : entries) {
        suggestions.push_back(entry.word);
    }

    return suggestions;
}

void AdvancedTrie::clear() {
    root = std::make_unique<TrieNode>();
    totalWords = 0;
}

size_t AdvancedTrie::getMemoryUsage() const {
    return calculateMemoryUsage(root.get());
}

size_t AdvancedTrie::calculateMemoryUsage(TrieNode* node) const {
    if (!node) return 0;

    size_t size = sizeof(TrieNode);

    if (node->wordData) {
        size += sizeof(MultiWordEntry);
        size += node->wordData->word.capacity();
        for (const auto& meaning : node->wordData->meanings) {
            size += meaning.capacity();
        }
        for (const auto& arabic : node->wordData->arabicTranslations) {
            size += arabic.capacity();
        }
        for (const auto& pos : node->wordData->partOfSpeeches) {
            size += pos.capacity();
        }
    }

    for (const auto& child : node->children) {
        size += calculateMemoryUsage(child.second.get());
    }

    return size;
}

void AdvancedTrie::optimizeMemory() {
    // Collect all words
    std::vector<MultiWordEntry> allWords;
    std::queue<std::pair<TrieNode*, std::string>> queue;
    queue.push({root.get(), ""});

    while (!queue.empty()) {
        auto [node, currentWord] = queue.front();
        queue.pop();

        if (node->isEndOfWord && node->wordData) {
            allWords.push_back(*node->wordData);
        }

        for (const auto& [ch, child] : node->children) {
            queue.push({child.get(), currentWord + ch});
        }
    }

    // Clear and rebuild
    clear();
    for (auto& entry : allWords) {
        entry.word.shrink_to_fit();
        for (auto& meaning : entry.meanings) {
            meaning.shrink_to_fit();
        }
        for (auto& arabic : entry.arabicTranslations) {
            arabic.shrink_to_fit();
        }
        for (auto& pos : entry.partOfSpeeches) {
            pos.shrink_to_fit();
        }
        insert(entry);
    }
}
