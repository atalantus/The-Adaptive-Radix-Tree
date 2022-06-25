#include "benchmark.h"

void art_insert(const std::vector<uint32_t>& numbers) {
    ART::art art;

    for (const auto& n: numbers) {
        art.insert(n);
    }
}

void art_search(const std::vector<uint32_t>& numbers) {
}

void art_range_search(const std::vector<uint32_t>& numbers) {
}

void trie_insert(const std::vector<uint32_t>& numbers) {
    Trie::trie trie;

    for (const auto& n: numbers) {
        trie.insert(n);
    }
}

void trie_search(const std::vector<uint32_t>& numbers) {
}

void trie_range_search(const std::vector<uint32_t>& numbers) {
}
