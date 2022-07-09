#include "node.h"

namespace art
{
    Node* Node256::Insert(const uint8_t partial_key, Node* child_node)
    {
        children_[partial_key] = child_node;
        return this;
    }

    Node* Node256::FindChild(const uint8_t partial_key) const
    {
        return children_[partial_key];
    }
}
