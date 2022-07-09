#include "node.h"

namespace art
{
    Node* Node48::Insert(const uint8_t partial_key, Node* child_node)
    {
        if (IsFull())
        {
            auto new_node = new Node256();

            for (uint16_t i = 0; i < 256; ++i)
            {
                if (keys_[i] == free_marker_) continue;

                new_node->children_[i] = children_[keys_[i]];
            }

            new_node->child_count_ = 48;

            return new_node->Insert(partial_key, child_node);
        }

        // insert
        // TODO: What about deletion?
        keys_[partial_key] = child_count_;
        children_[child_count_] = child_node;
        ++child_count_;

        return this;
    }

    Node* Node48::FindChild(const uint8_t partial_key) const
    {
        if (keys_[partial_key] != free_marker_)
            return children_[keys_[partial_key]];

        return nullptr;
    }
}
