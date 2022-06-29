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
}
