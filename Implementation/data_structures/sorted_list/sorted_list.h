#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

namespace sorted_list
{
    class SortedList
    {
    public:
        SortedList(uint32_t size) : sorted_list_(size)
        {
        }

        /**
         * Inserts a 32 bit value into the sorted_list.
         *
         * @param value the value to insert
         */
        void Insert(uint32_t value);

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
