#include "sorted_list.h"

namespace sorted_list
{
    bool SortedList::Find(const uint32_t value) const
    {
        return std::ranges::binary_search(sorted_list_, value);
    }

    std::vector<uint32_t> SortedList::FindRange(const uint32_t from, const uint32_t to) const
    {
        return {};
    }
}
