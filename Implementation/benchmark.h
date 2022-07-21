#pragma once

class Benchmark
{
public:
    virtual ~Benchmark() = default;

    virtual void InitializeStructure() = 0;

    virtual void DeleteStructure() = 0;

    virtual void Insert(std::vector<uint32_t> numbers) = 0;

    virtual void Search(std::vector<uint32_t> numbers, std::vector<bool>& expected) = 0;

    virtual void RangeSearch(std::vector<uint32_t> numbers, std::vector<std::vector<uint32_t>>& expected) = 0;
};
