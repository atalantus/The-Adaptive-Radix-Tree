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

    void Insert(const std::vector<uint32_t> numbers) override
    {
        for (uint32_t i = 0; i < numbers.size(); ++i)
            mtrie_->Insert(numbers[i]);
    }

    void Search(const std::vector<uint32_t> numbers, std::vector<bool>& expected) override
    {
        for (uint32_t i = 0; i < numbers.size(); ++i)
            if (mtrie_->Find(numbers[i]) != expected[i])
                std::cout << "M-Trie Search error: expected " << expected[i] << " got " << !expected[i] << " number " << numbers[i] <<
                    std::endl;
    }

    void RangeSearch(const std::vector<uint32_t> numbers, std::vector<std::vector<uint32_t>>& expected) override
    {
        for (uint32_t i = 0; i < 2 * numbers.size(); i += 2)
        {
            const auto actual = mtrie_->FindRange(numbers[i], numbers[i + 1]);

            if (actual.size() != expected[i / 2].size())
                std::cout << "M-Trie RangeSearch size error: expected " << expected[i].size() << " got " << actual.size() << std::endl;

            for (size_t j = 0; j < actual.size(); ++j)
                if (actual[j] != expected[i / 2][j])
                    std::cout << "M-Trie RangeSearch error: expected " << expected[i / 2][j] << " got " << actual[j] << " at position " << j <<
                        std::endl;
        }
    }

private:
    mtrie::MTrie* mtrie_ = nullptr;
};
