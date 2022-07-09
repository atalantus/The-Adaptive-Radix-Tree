#include "node.h"

namespace art
{
    Node* Node::Insert(const uint8_t partial_key, Node* child_node)
    {
        switch (type_)
        {
            case kNode4:
                {
                    const auto n = static_cast<Node4*>(this);
                    return n->Insert(partial_key, child_node);
                }
            case kNode16:
                {
                    const auto n = static_cast<Node16*>(this);
                    return n->Insert(partial_key, child_node);
                }
            case kNode48:
                {
                    const auto n = static_cast<Node48*>(this);
                    return n->Insert(partial_key, child_node);
                }
            case kNode256:
                {
                    const auto n = static_cast<Node256*>(this);
                    return n->Insert(partial_key, child_node);
                }
        }

        __unreachable();
    }

    Node* Node::FindChild(const uint8_t partial_key)
    {
        switch (type_)
        {
            case kNode4:
                {
                    const auto n = static_cast<Node4*>(this);
                    return n->FindChild(partial_key);
                }
            case kNode16:
                {
                    const auto n = static_cast<Node16*>(this);
                    return n->FindChild(partial_key);
                }
            case kNode48:
                {
                    const auto n = static_cast<Node48*>(this);
                    return n->FindChild(partial_key);
                }
            case kNode256:
                {
                    const auto n = static_cast<Node256*>(this);
                    return n->FindChild(partial_key);
                }
        }

        __unreachable();
    }

    bool Node::IsFull() const
    {
        switch (type_)
        {
            case kNode4: return child_count_ == 4;
            case kNode16: return child_count_ == 16;
            case kNode48: return child_count_ == 48;
            case kNode256: return false;
        }

        __unreachable();
    }

    bool Node::IsLazyExpanded(const uint64_t address_value)
    {
        return address_value & 0x7ULL;
    }

    bool Node::CmpLazyExpansion(const uint64_t address_value, const uint32_t key)
    {
        // address_value is actual full key value instead of address
        // (key value stored at high 32 bits)
        const uint32_t full_key_value = address_value >> 32;
        if (full_key_value == key)
            return true;
        // full key value is different
        return false;
    }
}
