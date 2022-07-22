#pragma once

#include "../../data_structures/mtrie/mtrie.h"
#include "../benchmark.h"

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

    void Insert(const std::vector<uint32_t>& numbers) override
    {
        for (uint32_t i = 0; i < numbers.size(); ++i)
            mtrie_->Insert(numbers[i]);
    }

    void Search(const std::vector<uint32_t>& numbers) override
    {
        for (uint32_t i = 0; i < numbers.size(); ++i)
            mtrie_->Find(numbers[i]);
    }

    void RangeSearch(const std::vector<uint32_t>& numbers) override
    {
        for (uint32_t i = 0; i < numbers.size(); i += 2)
            mtrie_->FindRange(numbers[i], numbers[i + 1]);
    }

private:
    mtrie::MTrie* mtrie_ = nullptr;
};
