#include "benchmark.h"

inline void ArtInsert(const std::vector<uint32_t>& numbers) {
    art::Art art;

    for (const auto& n: numbers) {
        art.Insert(n);
    }
}

inline void ArtSearch(const std::vector<uint32_t>& numbers) {}

inline void ArtRangeSearch(const std::vector<uint32_t>& numbers) {}

inline void TrieInsert(const std::vector<uint32_t>& numbers) {
    trie::Trie trie;

    for (const auto& n: numbers) {
        trie.Insert(n);
    }
}

inline void TrieSearch(const std::vector<uint32_t>& numbers) {}

inline void TrieRangeSearch(const std::vector<uint32_t>& numbers) {}
