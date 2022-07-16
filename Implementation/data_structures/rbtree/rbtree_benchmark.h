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

    void Insert(const uint32_t* numbers, const uint32_t size) override
    {
        for (uint32_t i = 0; i < size; ++i)
            rbtree_->Insert(numbers[i]);
    }

    void Search(const uint32_t* numbers, const uint32_t size) override
    {
        for (uint32_t i = 0; i < size; ++i)
            rbtree_->Find(numbers[i]);
    }

    void RangeSearch(const uint32_t* numbers, const uint32_t size) override
    {
        for (uint32_t i = 0; i < 2 * size; i += 2)
            rbtree_->FindRange(numbers[i], numbers[i + 1]);
    }

private:
    rbtree::RbTree* rbtree_ = nullptr;
};
