#pragma once

#include "htrie.h"
#include "../../benchmark.h"

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

    void Insert(const uint32_t* numbers, const uint32_t size) override
    {
        for (uint32_t i = 0; i < size; ++i)
            htrie_->Insert(numbers[i]);
    }

    void Search(const uint32_t* numbers, const uint32_t size) override
    {
        for (uint32_t i = 0; i < size; ++i)
            htrie_->Find(numbers[i]);
    }

    void RangeSearch(const uint32_t* numbers, const uint32_t size) override
    {
        // H-Trie doesn't support range queries
    }

private:
    htrie::HTrie* htrie_ = nullptr;
};
