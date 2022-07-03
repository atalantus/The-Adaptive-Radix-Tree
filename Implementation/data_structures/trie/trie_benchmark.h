#pragma once

#include "trie.h"
#include "../../benchmark.h"

class TrieBenchmark : public Benchmark
{
public:
    ~TrieBenchmark() override
    {
        delete trie_;
    }

    void InitializeStructure() override
    {
        trie_ = new trie::Trie();
    }

    void DeleteStructure() override
    {
        delete trie_;
        trie_ = nullptr;
    }

    void Insert(const uint32_t* numbers, const uint32_t size) override
    {
        auto& trie = *trie_;

        for (uint32_t i = 0; i < size; ++i) {
            trie.Insert(numbers[i]);

            if ((i+1)%1'600'000==0)
                std::cout << (i+1.0)/16000000*100 << "%" << std::endl;
        }
    }

    void Search(const uint32_t* numbers, const uint32_t size) override
    {
        const auto& trie = *trie_;

        for (uint32_t i = 0; i < size; ++i)
            trie.Find(numbers[i]);
    }

    void RangeSearch(const uint32_t* numbers, const uint32_t size) override
    {
        if (size > 5'000'000) return;

        const auto& trie = *trie_;

        for (uint32_t i = 0; i < 2 * size; ++i)
            trie.FindRange(numbers[i], numbers[++i]);
    }

private:
    trie::Trie* trie_ = nullptr;
};
