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

        SortedList(const uint32_t* numbers, const uint32_t size) : sorted_list_(numbers, numbers + size)
        {
            std::ranges::sort(sorted_list_);
        }

        bool Find(uint32_t value) const;

        std::vector<uint32_t> FindRange(uint32_t from, uint32_t to) const;

    private:
        std::vector<uint32_t> sorted_list_;
    };
}
