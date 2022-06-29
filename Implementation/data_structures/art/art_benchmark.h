#pragma once

#include "art.h"
#include "../../benchmark.h"

class ArtBenchmark : public Benchmark
{
public:
    ArtBenchmark() : art_{nullptr}
    {
    }

    ~ArtBenchmark() override
    {
        delete art_;
    }

    void InitializeStructure() override
    {
        art_ = new art::Art();
    }

    void DeleteStructure() override
    {
        delete art_;
        art_ = nullptr;
    }

    void Insert(const uint32_t* numbers, const uint32_t size) override
    {
        auto& art = *art_;

        for (uint32_t i = 0; i < size; ++i)
            art.Insert(numbers[i]);
    }

    void Search(const uint32_t* numbers, const uint32_t size) override
    {
        const auto& art = *art_;

        for (uint32_t i = 0; i < size; ++i)
            art.Find(numbers[i]);
    }

    void RangeSearch(const uint32_t* numbers, const uint32_t size) override
    {
        const auto& art = *art_;

        for (uint32_t i = 0; i < 2 * size; ++i)
            art.FindRange(numbers[i], numbers[++i]);
    }

private:
    art::Art* art_;
};
