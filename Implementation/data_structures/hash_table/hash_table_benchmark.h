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

    void Insert(const std::vector<uint32_t> numbers) override
    {
        for (uint32_t i = 0; i < numbers.size(); ++i)
            hash_table_->Insert(numbers[i]);
    }

    void Search(const std::vector<uint32_t> numbers, std::vector<bool>& expected) override
    {
        for (uint32_t i = 0; i < numbers.size(); ++i) {
            if (hash_table_->Find(numbers[i]) != expected[i])
                std::cout << "Hash-Table Search error: expected " << expected[i] << " got " << !expected[i] << " number " << numbers[i] <<
                    std::endl;
        }
    }

    void RangeSearch(const std::vector<uint32_t> numbers, std::vector<std::vector<uint32_t>>& expected) override
    {
        // Hash-Table doesn't support range queries
    }

private:
    hash_table::HashTable* hash_table_ = nullptr;
};
