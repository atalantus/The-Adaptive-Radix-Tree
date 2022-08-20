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

    void Search(const std::vector<uint32_t>& numbers, std::vector<bool>& expected) override
    {
        for (uint32_t i = 0; i < numbers.size(); ++i)
        {
            auto leaf = lookup(art_, (uint8_t*)&numbers[i]);
            if ((reinterpret_cast<uint64_t>(leaf) & 1) != expected[i])
                std::cerr << "\033[1;31mART (Leis) Search error: expected " << expected[i] << " got " << !expected[i] << " number " << std::hex
                    << numbers[i] << "\033[0m" << std::endl;
        }
    }

    void RangeSearch(const std::vector<uint32_t>& numbers, std::vector<std::vector<uint32_t>>& expected) override
    {
        for (uint32_t i = 0; i < numbers.size(); i += 2)
            ; // TODO
    }

private:
    Node* art_ = nullptr;
};