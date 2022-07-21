#pragma once

#include <cstdint>
#include <vector>

namespace sorted_list
{
    class SortedList
    {
    public:
        SortedList() = default;

        void Insert(const std::vector<uint32_t>& numbers);

        bool Find(uint32_t value) const;

        std::vector<uint32_t> FindRange(uint32_t from, uint32_t to) const;

    private:
        std::vector<uint32_t> sorted_list_;
    };
}
