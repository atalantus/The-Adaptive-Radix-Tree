#pragma once

#include "art.h"
#include "../../benchmark.h"

class ArtBenchmark : public Benchmark
{
public:
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

    void Insert(const std::vector<uint32_t>& numbers) override
    {
        for (uint32_t i = 0; i < numbers.size(); ++i)
            art_->Insert(numbers[i]);
    }

    void Search(const std::vector<uint32_t>& numbers) override
    {
        for (uint32_t i = 0; i < numbers.size(); ++i)
            art_->Find(numbers[i]);
    }

    void RangeSearch(const std::vector<uint32_t>& numbers) override
    {
        for (uint32_t i = 0; i < numbers.size(); i += 2)
            art_->FindRange(numbers[i], numbers[i + 1]);
    }

private:
    art::Art* art_ = nullptr;
};
