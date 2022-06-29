#pragma once

#include "ctrie.h"
#include "../../benchmark.h"

class CTrieBenchmark : public Benchmark
{
public:
    CTrieBenchmark() : ctrie_{nullptr}
    {
    }

    ~CTrieBenchmark() override
    {
        delete ctrie_;
    }

    void InitializeStructure() override
    {
        ctrie_ = new ctrie::CTrie();
    }

    void DeleteStructure() override
    {
        delete ctrie_;
        ctrie_ = nullptr;
    }

    void Insert(const uint32_t* numbers, const uint32_t size) override
    {
        auto& ctrie = *ctrie_;

        for (uint32_t i = 0; i < size; ++i)
            ctrie.Insert(numbers[i]);
    }

    void Search(const uint32_t* numbers, const uint32_t size) override
    {
        const auto& ctrie = *ctrie_;

        for (uint32_t i = 0; i < size; ++i)
            ctrie.Find(numbers[i]);
    }

    void RangeSearch(const uint32_t* numbers, const uint32_t size) override
    {
        const auto& ctrie = *ctrie_;

        for (uint32_t i = 0; i < 2 * size; ++i)
            ctrie.FindRange(numbers[i], numbers[++i]);
    }

private:
    ctrie::CTrie* ctrie_;
};
