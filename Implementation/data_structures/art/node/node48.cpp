#include "node.h"

namespace art
{
    void Node48::Insert(uint8_t partial_key, Node* node)
    {
    }

    Node* Node48::FindChild(const uint8_t partial_key) const
    {
        if (keys_[partial_key] != free_marker_)
            return children_[keys_[partial_key]];

        return nullptr;
    }
}
