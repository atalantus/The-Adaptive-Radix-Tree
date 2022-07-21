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

    void Insert(const std::vector<uint32_t> numbers) override
    {
        for (uint32_t i = 0; i < numbers.size(); ++i)
            htrie_->Insert(numbers[i]);
    }

    void Search(const std::vector<uint32_t> numbers, std::vector<bool>& expected) override
    {
        for (uint32_t i = 0; i < numbers.size(); ++i)
            if (htrie_->Find(numbers[i]) != expected[i])
                std::cout << "H-Trie Search error: expected " << expected[i] << " got " << !expected[i] << " number " << numbers[i] <<
                    std::endl;
    }

    void RangeSearch(const std::vector<uint32_t> numbers, std::vector<std::vector<uint32_t>>& expected) override
    {
        // H-Trie doesn't support range queries
    }

private:
    htrie::HTrie* htrie_ = nullptr;
};
