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

    void Search(const std::vector<uint32_t>& numbers) override
    {
        for (uint32_t i = 0; i < numbers.size(); ++i)
            trie_->Find(numbers[i]);
    }

    void RangeSearch(const std::vector<uint32_t>& numbers) override
    {
        for (uint32_t i = 0; i < numbers.size(); i += 2)
            trie_->FindRange(numbers[i], numbers[i + 1]);
    }

private:
    trie::Trie* trie_ = nullptr;
};
