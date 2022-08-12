#pragma once

#include "../../data_structures/art_leis/ARTshort.cpp"
#include "../benchmark.h"

class ArtLeisBenchmark : public Benchmark
{
public:
    ~ArtLeisBenchmark() override
    {
        delete art_;
    }

    void InitializeStructure() override
    {
        art_ = nullptr;
    }

    void DeleteStructure() override
    {
        delete art_;
        art_ = nullptr;
    }

    void Insert(const std::vector<uint32_t>& numbers) override
    {
        for (uint32_t i = 0; i < numbers.size(); ++i)
            insert(art_, &art_, (uint8_t*)&numbers[i], numbers[i]);
    }

    void Search(const std::vector<uint32_t>& numbers) override
    {
        for (uint32_t i = 0; i < numbers.size(); ++i)
            lookup(art_, (uint8_t*)&numbers[i]);
    }

    void RangeSearch(const std::vector<uint32_t>& numbers) override
    {
        for (uint32_t i = 0; i < numbers.size(); i += 2)
            ; // TODO
    }

private:
    Node* art_ = nullptr;
};
