#pragma once

#include "../../data_structures/sorted_list/sorted_list.h"
#include "../benchmark.h"

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

    void Insert(const std::vector<uint32_t>& numbers) override
    {
        sorted_list_->Insert(numbers);
    }

    void Search(const std::vector<uint32_t>& numbers, std::vector<bool>& expected) override
    {
        for (uint32_t i = 0; i < numbers.size(); ++i)
            expected.push_back(sorted_list_->Find(numbers[i]));
    }

    void RangeSearch(const std::vector<uint32_t>& numbers, std::vector<std::vector<uint32_t>>& expected) override
    {
        for (uint32_t i = 0; i < numbers.size(); i += 2)
            expected.push_back(sorted_list_->FindRange(numbers[i], numbers[i + 1]));
    }

private:
    sorted_list::SortedList* sorted_list_ = nullptr;
};