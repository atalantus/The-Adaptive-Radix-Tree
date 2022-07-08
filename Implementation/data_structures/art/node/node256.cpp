#include "node.h"

namespace art
{
    void Node256::Insert(uint8_t partial_key, Node* node)
    {
    }

    Node* Node256::FindChild(const uint8_t partial_key) const
    {
        return children_[partial_key];
    }
}
