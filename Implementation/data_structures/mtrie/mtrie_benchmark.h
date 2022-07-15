#pragma once

#include "mtrie.h"
#include "../../benchmark.h"

class MTrieBenchmark : public Benchmark
{
public:
    ~MTrieBenchmark() override
    {
        delete mtrie_;
    }

    void InitializeStructure() override
    {
        mtrie_ = new mtrie::MTrie();
    }

    void DeleteStructure() override
    {
        delete mtrie_;
        mtrie_ = nullptr;
    }

    void Insert(const uint32_t* numbers, const uint32_t size) override
    {
        for (uint32_t i = 0; i < size; ++i)
            mtrie_->Insert(numbers[i]);
    }

    void Search(const uint32_t* numbers, const uint32_t size) override
    {
        for (uint32_t i = 0; i < size; ++i)
            mtrie_->Find(numbers[i]);
    }

    void RangeSearch(const uint32_t* numbers, const uint32_t size) override
    {
        for (uint32_t i = 0; i < 2 * size; ++i)
            mtrie_->FindRange(numbers[i], numbers[++i]);
    }

private:
    mtrie::MTrie* mtrie_ = nullptr;
};
