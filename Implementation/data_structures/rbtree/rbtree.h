#pragma once

#include <cstdint>
#include <set>

namespace rbtree
{
    class RbTree
    {
    public:
        RbTree() = default;

        void Insert(uint32_t value);

        bool Find(uint32_t value) const;

    private:
        std::set<uint32_t> rbtree_;
    };
}
