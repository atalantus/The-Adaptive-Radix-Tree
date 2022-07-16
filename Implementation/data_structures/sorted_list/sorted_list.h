#pragma once

#include <cstdint>
#include <vector>
#include <algorithm>

namespace sorted_list
{
    class SortedList
    {
    public:
        SortedList() = default;

        void Insert(const uint32_t* numbers, uint32_t size);

        bool Find(uint32_t value) const;

        std::vector<uint32_t> FindRange(uint32_t from, uint32_t to) const;

    private:
        std::vector<uint32_t> sorted_list_;
    };
}
