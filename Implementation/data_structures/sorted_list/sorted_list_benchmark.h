#pragma once

#include "sorted_list.h"
#include "../../benchmark.h"

class SortedListBenchmark : public Benchmark
{
public:
    ~SortedListBenchmark() override
    {
        delete sorted_list_;
    }

    void InitializeStructure() override
    {
        sorted_list_ = new sorted_list::SortedList();
    }

    void DeleteStructure() override
    {
        delete sorted_list_;
        sorted_list_ = nullptr;
    }

    void Insert(const uint32_t* numbers, const uint32_t size) override
    {
        sorted_list_->Insert(numbers, size);
    }

    void Search(const uint32_t* numbers, const uint32_t size) override
    {
        for (uint32_t i = 0; i < size; ++i)
            sorted_list_->Find(numbers[i]);
    }

    void RangeSearch(const uint32_t* numbers, const uint32_t size) override
    {
        for (uint32_t i = 0; i < 2 * size; ++i)
            sorted_list_->FindRange(numbers[i], numbers[++i]);
    }

private:
    sorted_list::SortedList* sorted_list_ = nullptr;
};
