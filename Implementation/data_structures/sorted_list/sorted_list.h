#pragma once

#include <cstdint>
#include <vector>
#include <algorithm>

namespace sorted_list
{
    class SortedList
    {
    public:
        SortedList(const uint32_t* numbers, const uint32_t size) : sorted_list_(numbers, numbers + size)
        {
        }

        /**
         * Checks if a 32 bit value exists in the sorted_list via binary search.
         *
         * @param value the value to search for
         * @return true if the value exists otherwise false
         */
        bool Find(uint32_t value) const;

        /**
         * Finds all 32 bit values in the ctrie within a given range.
         *
         * @param from the value to search from (inclusive)
         * @param to the value to search to (inclusive)
         * @return vector of values sorted in ascending order
         */
        std::vector<uint32_t> FindRange(uint32_t from, uint32_t to) const;

    private:
        std::vector<uint32_t> sorted_list_;
    };
}
