#pragma once

#include "data_structures/data_structures.h"

/**
 * For more accurate benchmarking results we do not want to use any
 * polymorphic approaches towards testing different index structures.
 * Even though it would lead to cleaner code overall the dynamic dispatch 
 * would introduce a significant performance overhead.
 */

inline void ArtInsert(art::Art* art, const uint32_t* numbers, uint32_t size);

inline void ArtSearch(const art::Art* art, const uint32_t* numbers, uint32_t size);

inline void ArtRangeSearch(const art::Art* art, const std::vector<std::pair<uint32_t, uint32_t>>& numbers);

inline void TrieInsert(trie::Trie* trie, const uint32_t* numbers, uint32_t size);

inline void TrieSearch(const trie::Trie* trie, const uint32_t* numbers, uint32_t size);

inline void TrieRangeSearch(const trie::Trie* trie, const std::vector<std::pair<uint32_t, uint32_t>>& numbers);
