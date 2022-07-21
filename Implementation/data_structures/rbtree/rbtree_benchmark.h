#pragma once

#include "rbtree.h"
#include "../../benchmark.h"

class RbTreeBenchmark : public Benchmark
{
public:
    ~RbTreeBenchmark() override
    {
        delete rbtree_;
    }

    void InitializeStructure() override
    {
        rbtree_ = new rbtree::RbTree();
    }

    void DeleteStructure() override
    {
        delete rbtree_;
        rbtree_ = nullptr;
    }

    void Insert(const std::vector<uint32_t> numbers) override
    {
        for (uint32_t i = 0; i < numbers.size(); ++i)
            rbtree_->Insert(numbers[i]);
    }

    void Search(const std::vector<uint32_t> numbers, std::vector<bool>& expected) override
    {
        for (uint32_t i = 0; i < numbers.size(); ++i)
            if (rbtree_->Find(numbers[i]) != expected[i])
                std::cout << "RB-Tree Search error: expected " << expected[i] << " got " << !expected[i] << " number " << numbers[i] <<
                    std::endl;
    }

    void RangeSearch(const std::vector<uint32_t> numbers, std::vector<std::vector<uint32_t>>& expected) override
    {
        for (uint32_t i = 0; i < 2 * numbers.size(); i += 2)
        {
            const auto actual = rbtree_->FindRange(numbers[i], numbers[i + 1]);

            if (actual.size() != expected[i / 2].size())
                std::cout << "RB-Tree RangeSearch size error: expected " << expected[i].size() << " got " << actual.size() << std::endl;

            for (size_t j = 0; j < actual.size(); ++j)
                if (actual[j] != expected[i / 2][j])
                    std::cout << "RB-Tree RangeSearch error: expected " << expected[i / 2][j] << " got " << actual[j] << " at position " <<
                        j <<
                        std::endl;
        }
    }

private:
    rbtree::RbTree* rbtree_ = nullptr;
};
