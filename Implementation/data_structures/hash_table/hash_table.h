#pragma once

#include <cstdint>
#include <unordered_set>

namespace hash_table
{
    class HashTable
    {
    public:
        HashTable() = default;

        void Insert(uint32_t value);

        bool Find(uint32_t value) const;

    private:
        std::unordered_set<uint32_t> hash_table_;
    };
}
