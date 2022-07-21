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

    void Insert(const std::vector<uint32_t> numbers) override
    {
        for (uint32_t i = 0; i < numbers.size(); ++i)
            art_->Insert(numbers[i]);
    }

    void Search(const std::vector<uint32_t> numbers, std::vector<bool>& expected) override
    {
        for (uint32_t i = 0; i < numbers.size(); ++i)
        {
            if (art_->Find(numbers[i]) != expected[i])
                std::cout << "ART Search error: expected " << expected[i] << " got " << !expected[i] << " number " << numbers[i] <<
                    std::endl;
        }
    }

    void RangeSearch(const std::vector<uint32_t> numbers, std::vector<std::vector<uint32_t>>& expected) override
    {
        for (uint32_t i = 0; i < 2 * numbers.size(); i += 2)
        {
            const auto actual = art_->FindRange(numbers[i], numbers[i + 1]);

            if (actual.size() != expected[i / 2].size())
                std::cout << "ART RangeSearch size error: expected " << expected[i].size() << " got " << actual.size() << std::endl;

            for (size_t j = 0; j < actual.size(); ++j)
                if (actual[j] != expected[i / 2][j])
                    std::cout << "ART RangeSearch error: expected " << expected[i / 2][j] << " got " << actual[j] << " at position " << j <<
                        std::endl;
        }
    }

private:
    art::Art* art_ = nullptr;
};
