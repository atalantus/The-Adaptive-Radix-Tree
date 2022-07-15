#pragma once

#include <cstdint>
#include <set>
#include <vector>

namespace rbtree
{
    class RbTree
    {
    public:
        RbTree() = default;

        void Insert(uint32_t value);

        bool Find(uint32_t value) const;

        std::vector<uint32_t> FindRange(uint32_t from, uint32_t to) const;

    private:
        std::set<uint32_t> rbtree_;
    };
}
