#include "rbtree.h"

namespace rbtree
{
    void RbTree::Insert(const uint32_t value)
    {
        rbtree_.insert(value);
    }

    bool RbTree::Find(const uint32_t value) const
    {
        return rbtree_.find(value) != rbtree_.end();
    }

    std::vector<uint32_t> RbTree::FindRange(const uint32_t from, const uint32_t to) const
    {
        std::vector<uint32_t> res;

        auto it = rbtree_.find(from);

        if (it == rbtree_.end() || *it > to) return res;

        while (it != rbtree_.end() && *it <= to)
        {
            res.push_back(*it);
            ++it;
        }

        return res;
    }
}
