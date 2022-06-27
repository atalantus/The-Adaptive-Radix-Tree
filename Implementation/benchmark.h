#pragma once

#include "data_structures/data_structures.h"

/**
 * For more accurate benchmarking results we do not want to use any
 * polymorphic approaches towards testing different index structures.
 * Even though it would lead to cleaner code overall the dynamic dispatch 
 * would introduce a significant performance overhead.
 */

inline void ArtInsert(art::Art* art, const uint32_t* numbers, const uint32_t size)
{
    for (uint32_t i = 0; i < size; ++i)
        art->Insert(numbers[i]);
}

inline void ArtSearch(const art::Art* art, const uint32_t* numbers, const uint32_t size)
{
    for (uint32_t i = 0; i < size; ++i)
        art->Find(numbers[i]);
}

inline void ArtRangeSearch(const art::Art* art, const std::vector<std::pair<uint32_t, uint32_t>>& numbers)
{
    for (const auto& [from, to] : numbers)
        // TODO: Check once if correct results
        art->FindRange(from, to);
}

inline void TrieInsert(trie::Trie* trie, const uint32_t* numbers, const uint32_t size)
{
    if (size > 5'000'000) return;

    for (uint32_t i = 0; i < size; ++i)
        trie->Insert(numbers[i]);
}

inline void TrieSearch(const trie::Trie* trie, const uint32_t* numbers, const uint32_t size)
{
    if (size > 5'000'000) return;

    for (uint32_t i = 0; i < size; ++i)
        trie->Find(numbers[i]);
}

inline void TrieRangeSearch(const trie::Trie* trie, const std::vector<std::pair<uint32_t, uint32_t>>& numbers)
{
    for (const auto& [from, to] : numbers)
        // TODO: Check once if correct results
        trie->FindRange(from, to);
}

inline void CTrieInsert(ctrie::CTrie* ctrie, const uint32_t* numbers, const uint32_t size)
{
    for (uint32_t i = 0; i < size; ++i)
        ctrie->Insert(numbers[i]);
}

inline void CTrieSearch(const ctrie::CTrie* ctrie, const uint32_t* numbers, const uint32_t size)
{
    for (uint32_t i = 0; i < size; ++i)
        ctrie->Find(numbers[i]);
}

inline void CTrieRangeSearch(const ctrie::CTrie* ctrie, const std::vector<std::pair<uint32_t, uint32_t>>& numbers)
{
    for (const auto& [from, to] : numbers)
        // TODO: Check once if correct results
        ctrie->FindRange(from, to);
}

inline void SortedListSearch(const sorted_list::SortedList* list, const uint32_t* numbers, const uint32_t size)
{
    for (uint32_t i = 0; i < size; ++i)
        list->Find(numbers[i]);
}

inline void SortedListSearch(const sorted_list::SortedList* list,
                             const std::vector<std::pair<uint32_t, uint32_t>>& numbers)
{
    for (const auto& [from, to] : numbers)
        // TODO: Check once if correct results
        list->FindRange(from, to);
}
