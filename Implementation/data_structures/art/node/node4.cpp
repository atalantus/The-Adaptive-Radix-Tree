#include "node.h"

namespace art
{
    void Node4::Insert(uint8_t partial_key, Node* node)
    {
    }

    Node* Node4::FindChild(const uint8_t partial_key) const
    {
        for (uint8_t i = 0; i < 4; ++i)
            if (keys_[i] == partial_key)
                return children_[i];

        return nullptr;
    }
}
