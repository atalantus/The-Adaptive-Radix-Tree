#include "sorted_list.h"

namespace sorted_list
{
    bool SortedList::Find(const uint32_t value) const
    {
        return std::ranges::binary_search(sorted_list_, value);
    }

    std::vector<uint32_t> SortedList::FindRange(const uint32_t from, const uint32_t to) const
    {
        std::vector<uint32_t> res;

        auto it = std::ranges::lower_bound(sorted_list_, from);

        if (it == sorted_list_.end() || *it > to) return res;

        while (it != sorted_list_.end() && *it <= to) {
            res.push_back(*it);
            ++it;
        }

        return res;
    }
}
