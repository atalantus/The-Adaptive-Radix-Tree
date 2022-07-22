#pragma once

#include "../../data_structures/trie/trie.h"
#include "../benchmark.h"

class TrieBenchmark : public Benchmark
{
public:
    ~TrieBenchmark() override
    {
        delete trie_;
    }

    void InitializeStructure() override
    {
        trie_ = new trie::Trie();
    }

    void DeleteStructure() override
    {
        delete trie_;
        trie_ = nullptr;
    }

    void Insert(const std::vector<uint32_t>& numbers) override
    {
        for (uint32_t i = 0; i < numbers.size(); ++i)
            trie_->Insert(numbers[i]);
    }

    void Search(const std::vector<uint32_t>& numbers, std::vector<bool>& expected) override
    {
        for (uint32_t i = 0; i < numbers.size(); ++i)
        {
            if (trie_->Find(numbers[i]) != expected[i])
                std::cerr << "\033[1;31mTrie Search error: expected " << expected[i] << " got " << !expected[i] << " number " << std::hex
                    << numbers[i] << "\033[0m" << std::endl;
        }
    }

    void RangeSearch(const std::vector<uint32_t>& numbers, std::vector<std::vector<uint32_t>>& expected) override
    {
        for (uint32_t i = 0; i < numbers.size(); i += 2)
        {
            const auto actual = trie_->FindRange(numbers[i], numbers[i + 1]);

            if (actual.size() != expected[i / 2].size())
                std::cerr << "\033[1;31mTrie RangeSearch size error: expected " << expected[i / 2].size() << " got " << actual.size() <<
                    " at set " << i / 2 << "\033[0m" << std::endl;

            size_t j = 0;
            for (; j < std::min(actual.size(), expected[i / 2].size()); ++j)
                if (actual[j] != expected[i / 2][j])
                    std::cerr << "\033[1;31mTrie RangeSearch error: expected " << std::hex << expected[i / 2][j] << " got " << actual[j] <<
                        " at position " << std::dec << j << " of set " << i / 2 << "\033[0m" << std::endl;

            if (actual.size() > expected[i / 2].size())
                for (; j < actual.size(); ++j)
                    std::cerr << "\033[1;31mTrie RangeSearch error: actual left over " << std::hex << actual[j] <<
                        " at position " << std::dec << j << " of set " << i / 2 << "\033[0m" << std::endl;
            else if (actual.size() < expected[i / 2].size())
                for (; j < expected[i / 2].size(); ++j)
                    std::cerr << "\033[1;31mTrie RangeSearch error: expected left over " << std::hex << expected[i / 2][j] <<
                        " at position " << std::dec << j << " of set " << i / 2 << "\033[0m" << std::endl;
        }
    }

private:
    trie::Trie* trie_ = nullptr;
};