#pragma once

#include "data_structures/data_structures.h"

/**
 * For more accurate benchmarking results we do not want to use any
 * polymorphic approaches towards testing different index structures.
 * Even though it would lead to cleaner code overall the dynamic dispatch 
 * would introduce a significant performance overhead.
 */

void art_insert(const std::vector<uint32_t>& numbers);

void art_search(const std::vector<uint32_t>& numbers);

void art_range_search(const std::vector<uint32_t>& numbers);

void trie_insert(const std::vector<uint32_t>& numbers);

void trie_search(const std::vector<uint32_t>& numbers);

void trie_range_search(const std::vector<uint32_t>& numbers);
