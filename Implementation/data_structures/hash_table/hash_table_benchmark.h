#pragma once

#include "hash_table.h"
#include "../../benchmark.h"

class HashTableBenchmark : public Benchmark
{
public:
    ~HashTableBenchmark() override
    {
        delete hash_table_;
    }

    void InitializeStructure() override
    {
        hash_table_ = new hash_table::HashTable();
    }

    void DeleteStructure() override
    {
        delete hash_table_;
        hash_table_ = nullptr;
    }

    void Insert(const uint32_t* numbers, const uint32_t size) override
    {
        for (uint32_t i = 0; i < size; ++i)
            hash_table_->Insert(numbers[i]);
    }

    void Search(const uint32_t* numbers, const uint32_t size) override
    {
        for (uint32_t i = 0; i < size; ++i)
            hash_table_->Find(numbers[i]);
    }

    void RangeSearch(const uint32_t* numbers, const uint32_t size) override
    {
        // Hash-Table doesn't support range queries
    }

private:
    hash_table::HashTable* hash_table_ = nullptr;
};
