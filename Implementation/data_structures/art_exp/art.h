#pragma once

#include <cstdint>
#include <vector>
#include "node/node.h"

namespace art_exp
{
    class Art
    {
    public:
        Art() : root_{new Node4()}
        {
        }

        ~Art()
        {
            root_->Destruct();
        }

        void Insert(uint32_t value);

        bool Find(uint32_t value) const;

        std::vector<uint32_t> FindRange(uint32_t from, uint32_t to) const;

        /**
         * Prints the Tree in pre-order.
         */
        void PrintTree() const;

    private:
        void ExpandLazyExpansion(uint32_t value1, uint32_t value2, int depth, Node* node);

    private:
        Node* root_;
    };
}
