#pragma once

#include "data_structures/data_structures.h"

/**
 * For more accurate benchmarking results we do not want to use any
 * polymorphic approaches towards testing different index structures.
 * Even though it would lead to cleaner code overall the dynamic dispatch 
 * would introduce a significant performance overhead.
 */

inline void ArtInsert(const std::vector<uint32_t>& numbers);

inline void ArtSearch(const std::vector<uint32_t>& numbers);

inline void ArtRangeSearch(const std::vector<uint32_t>& numbers);

inline void TrieInsert(const std::vector<uint32_t>& numbers);

inline void TrieSearch(const std::vector<uint32_t>& numbers);

inline void TrieRangeSearch(const std::vector<uint32_t>& numbers);
