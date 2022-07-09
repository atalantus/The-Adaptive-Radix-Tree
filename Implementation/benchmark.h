#pragma once

class Benchmark
{
public:
    virtual ~Benchmark() = default;

    virtual void InitializeStructure() = 0;

    virtual void DeleteStructure() = 0;

    virtual void Insert(const uint32_t* numbers, const uint32_t size) = 0;

    virtual void Search(const uint32_t* numbers, const uint32_t size) = 0;

    virtual void RangeSearch(const uint32_t* numbers, const uint32_t size) = 0;
};
