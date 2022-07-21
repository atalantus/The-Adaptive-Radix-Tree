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

    void Insert(const std::vector<uint32_t>& numbers) override
    {
        for (uint32_t i = 0; i < numbers.size(); ++i)
            rbtree_->Insert(numbers[i]);
    }

    void Search(const std::vector<uint32_t>& numbers) override
    {
        for (uint32_t i = 0; i < numbers.size(); ++i)
            rbtree_->Find(numbers[i]);
    }

    void RangeSearch(const std::vector<uint32_t>& numbers) override
    {
        for (uint32_t i = 0; i < numbers.size(); i += 2)
            rbtree_->FindRange(numbers[i], numbers[i + 1]);
    }

private:
    rbtree::RbTree* rbtree_ = nullptr;
};
