#include "sorted_list.h"

#include <algorithm>

namespace sorted_list
{
    void SortedList::Insert(const std::vector<uint32_t>& numbers)
    {
        sorted_list_ = std::vector(numbers);
        // sort vector
        std::ranges::sort(sorted_list_);
        // remove duplicates
        auto rng = std::ranges::unique(sorted_list_);
        sorted_list_.erase(rng.begin(), rng.end());
    }

    bool SortedList::Find(const uint32_t value) const
    {
        return std::ranges::binary_search(sorted_list_, value);
    }

    std::vector<uint32_t> SortedList::FindRange(const uint32_t from, const uint32_t to) const
    {
        std::vector<uint32_t> res;

        auto it = std::ranges::lower_bound(sorted_list_, from);

        if (it == sorted_list_.end() || *it > to) return res;

        while (it != sorted_list_.end() && *it <= to)
        {
            res.push_back(*it);
            ++it;
        }

        return res;
    }
}
