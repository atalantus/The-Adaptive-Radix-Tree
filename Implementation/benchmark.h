#pragma once

class Benchmark
{
public:
    virtual ~Benchmark() = default;

    virtual void InitializeStructure() = 0;

    virtual void DeleteStructure() = 0;

    virtual void Insert(const std::vector<uint32_t>& numbers) = 0;

    virtual void Search(const std::vector<uint32_t>& numbers, std::vector<bool>& expected) = 0;

    virtual void RangeSearch(const std::vector<uint32_t>& numbers, std::vector<std::vector<uint32_t>>& expected) = 0;
};
