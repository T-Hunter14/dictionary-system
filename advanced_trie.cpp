#include "advanced_trie.h"
#include <cctype>
#include <stack>
#include <utility>

// ---------------------------------------------------------------------------
// Internal helpers
// ---------------------------------------------------------------------------

// Build an alpha-only lowercase key from an arbitrary string_view.
// Non-alpha characters are skipped so "C++" → "c", "Hello!" → "hello".
static std::string makeKey(std::string_view word) noexcept {
    std::string key;
    key.reserve(word.size());
    for (char c : word) {
        if (std::isalpha(static_cast<unsigned char>(c)))
            key += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return key;
}

// ---------------------------------------------------------------------------
// Trie
// ---------------------------------------------------------------------------

Trie::Trie() : root_(new TrieNode{}) {}

Trie::~Trie() { destroyAll(root_); }

Trie::Trie(Trie&& other) noexcept : root_(other.root_) {
    other.root_ = nullptr;
}

Trie& Trie::operator=(Trie&& other) noexcept {
    if (this != &other) {
        destroyAll(root_);
        root_ = other.root_;
        other.root_ = nullptr;
    }
    return *this;
}

// Iterative post-order deletion — avoids stack overflow on large tries.
void Trie::destroyAll(TrieNode* root) noexcept {
    if (!root) return;
    std::stack<TrieNode*> st;
    st.push(root);
    while (!st.empty()) {
        TrieNode* n = st.top(); st.pop();
        for (int i = 0; i < 26; ++i)
            if (n->children[i]) st.push(n->children[i]);
        delete n;
    }
}

// ---------------------------------------------------------------------------
// insert
// ---------------------------------------------------------------------------
void Trie::insert(std::string_view word) noexcept {
    std::string key = makeKey(word);
    if (key.empty()) return;

    TrieNode* node = root_;
    for (char c : key) {
        int idx = c - 'a';
        if (!node->children[idx])
            node->children[idx] = new TrieNode{};
        node = node->children[idx];
    }
    node->isEnd = true;
}

// ---------------------------------------------------------------------------
// contains
// ---------------------------------------------------------------------------
bool Trie::contains(std::string_view word) const noexcept {
    std::string key = makeKey(word);
    TrieNode* node = walk(key);
    return node && node->isEnd;
}

// Walk trie for a pre-processed key string.
TrieNode* Trie::walk(std::string_view key) const noexcept {
    TrieNode* node = root_;
    for (char c : key) {
        int idx = c - 'a';
        if (!node->children[idx]) return nullptr;
        node = node->children[idx];
    }
    return node;
}

// ---------------------------------------------------------------------------
// autocomplete
// ---------------------------------------------------------------------------
// DFS using an explicit stack to avoid recursion overhead.
// Because children are iterated 0→25 ('a'→'z'), results come out
// lexicographically sorted for free.
std::vector<std::string> Trie::autocomplete(std::string_view prefix,
                                             int maxResults) const {
    std::vector<std::string> results;
    if (prefix.empty() || maxResults <= 0) return results;

    std::string key = makeKey(prefix);
    TrieNode* start = walk(key);
    if (!start) return results;

    // Stack stores (node, current-word-so-far).
    // We use the already-known prefix as the base string.
    std::stack<std::pair<TrieNode*, std::string>> st;
    st.push({start, std::string(key)});

    while (!st.empty() && static_cast<int>(results.size()) < maxResults) {
        auto [node, word] = std::move(st.top());
        st.pop();

        if (node->isEnd) {
            results.push_back(word);
            if (static_cast<int>(results.size()) >= maxResults) break;
        }

        // Push children in reverse order (25→0) so that when popped they
        // come out in forward alphabetical order ('a' first).
        for (int i = 25; i >= 0; --i) {
            if (node->children[i]) {
                st.push({node->children[i], word + static_cast<char>('a' + i)});
            }
        }
    }
    return results;
}

// ---------------------------------------------------------------------------
// remove  — recursive, prunes dead nodes
// ---------------------------------------------------------------------------
bool Trie::remove(std::string_view word) noexcept {
    std::string key = makeKey(word);
    if (key.empty()) return false;
    return removeHelper(root_, key, 0);
}

// Returns true when the caller should delete this node (it has no children
// and is not the end of any other word).
bool Trie::removeHelper(TrieNode* node, std::string_view key,
                         std::size_t depth) noexcept {
    if (!node) return false;

    if (depth == key.size()) {
        if (!node->isEnd) return false;   // word not in trie
        node->isEnd = false;
        // Prune this node if it has no children left.
        for (int i = 0; i < 26; ++i)
            if (node->children[i]) return false;
        return true;  // safe to delete
    }

    int idx = static_cast<int>(static_cast<unsigned char>(key[depth])) - 'a';
    if (idx < 0 || idx >= 26) return false;
    if (!node->children[idx]) return false;

    bool shouldDelete = removeHelper(node->children[idx], key, depth + 1);
    if (shouldDelete) {
        delete node->children[idx];
        node->children[idx] = nullptr;
        // Prune this node too if it became a dead leaf.
        if (!node->isEnd) {
            for (int i = 0; i < 26; ++i)
                if (node->children[i]) return false;
            return (node != root_);  // never delete the root
        }
    }
    return false;
}
