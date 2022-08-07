#pragma once

#include "../../data_structures/art_crtp/art.h"
#include "../benchmark.h"

class ArtCRTPBenchmark : public Benchmark
{
public:
    ~ArtCRTPBenchmark() override
    {
        delete art_;
    }

    void InitializeStructure() override
    {
        art_ = new art_crtp::Art();
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
    art_crtp::Art* art_ = nullptr;
};
