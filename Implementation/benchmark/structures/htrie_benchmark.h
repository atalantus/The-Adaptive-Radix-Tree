#pragma once

#include "../../data_structures/htrie/htrie.h"
#include "../benchmark.h"

class HTrieBenchmark : public Benchmark
{
public:
    ~HTrieBenchmark() override
    {
        delete htrie_;
    }

    void InitializeStructure() override
    {
        htrie_ = new htrie::HTrie();
    }

    void DeleteStructure() override
    {
        delete htrie_;
        htrie_ = nullptr;
    }

    void Insert(const std::vector<uint32_t>& numbers) override
    {
        for (uint32_t i = 0; i < numbers.size(); ++i)
            htrie_->Insert(numbers[i]);
    }

    void Search(const std::vector<uint32_t>& numbers) override
    {
        for (uint32_t i = 0; i < numbers.size(); ++i)
            htrie_->Find(numbers[i]);
    }

    void RangeSearch(const std::vector<uint32_t>&) override
    {
        // H-Trie doesn't support range queries
    }

private:
    htrie::HTrie* htrie_ = nullptr;
};
