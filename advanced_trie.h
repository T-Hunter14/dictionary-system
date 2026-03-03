#ifndef ADVANCED_TRIE_H
#define ADVANCED_TRIE_H

#include <string>
#include <string_view>
#include <vector>

// Array-based trie node: 26 raw pointers for 'a'-'z', zero hashing overhead.
struct TrieNode {
    TrieNode* children[26] = {};   // zero-initialised, one slot per lowercase letter
    bool isEnd = false;
};

class Trie {
public:
    Trie();
    ~Trie();

    // Non-copyable, movable
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
    Trie(Trie&&) noexcept;
    Trie& operator=(Trie&&) noexcept;

    // All methods accept string_view — no unnecessary copies.
    // Non-alpha characters in the key are silently skipped.
    void insert(std::string_view word) noexcept;
    bool contains(std::string_view word) const noexcept;

    // Returns up to maxResults completions in lexicographic order.
    std::vector<std::string> autocomplete(std::string_view prefix,
                                          int maxResults = 20) const;

    // Recursive delete that prunes dead nodes (frees memory).
    bool remove(std::string_view word) noexcept;

private:
    TrieNode* root_;

    // Helpers
    // Walks the trie for `key` (already lowercased, alpha-only).
    // Returns the terminal node, or nullptr if not found.
    TrieNode* walk(std::string_view key) const noexcept;

    // Recursive helper for remove; returns true when node can be freed.
    bool removeHelper(TrieNode* node, std::string_view key,
                      std::size_t depth) noexcept;

    // Iterative post-order deletion of all nodes.
    static void destroyAll(TrieNode* root) noexcept;
};

#endif
