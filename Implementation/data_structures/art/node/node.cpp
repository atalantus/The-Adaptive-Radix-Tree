#include "node.h"

namespace art
{
    Node* null_node = nullptr;

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

    Node*& Node::FindChild(const uint8_t partial_key)
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
            case kNode4:
                return child_count_ == 4;
            case kNode16:
                return child_count_ == 16;
            case kNode48:
                return child_count_ == 48;
            case kNode256:
                return false;
        }

        __unreachable();
    }

    void Node::PrintTree(int depth)
    {
        switch (type_)
        {
            case kNode4:
            {
                const auto n = static_cast<Node4*>(this);
                n->PrintTree(depth);
                return;
            }
            case kNode16:
            {
                const auto n = static_cast<Node16*>(this);
                n->PrintTree(depth);
                return;
            }
            case kNode48:
            {
                const auto n = static_cast<Node48*>(this);
                n->PrintTree(depth);
                return;
            }
            case kNode256:
            {
                const auto n = static_cast<Node256*>(this);
                n->PrintTree(depth);
                return;
            }
        }

        __unreachable();
    }

    void Node::PrintChild(Node* child, int i, int m)
    {
        if (child == nullptr)
            return;
        else if (Node::IsLazyExpanded(child))
        {
            auto val = static_cast<uint32_t>(reinterpret_cast<uint64_t>(child) >> 32);
            if (!IsBigEndian()) val = SwapEndianess(val);
            std::cout << std::dec << i << ":[" << std::hex << val << "]";
        }
        else
            std::cout << std::dec << i << ":" << std::hex << child;
        if (i < m - 1)
            std::cout << ",";
    }

    bool Node::IsLazyExpanded(Node* node_ptr)
    {
        return reinterpret_cast<uint64_t>(node_ptr) & 0x7ULL;
    }

    bool Node::CmpLazyExpansion(Node* node_ptr, const uint32_t key)
    {
        // address_value is actual full key value instead of address
        // (key value stored at high 32 bits)
        const uint32_t full_key_value = reinterpret_cast<uint64_t>(node_ptr) >> 32;
        if (full_key_value == key)
            return true;
        // full key value is different
        return false;
    }

    void Node::Destruct()
    {
        switch (type_)
        {
            case kNode4:
            {
                const auto n = static_cast<Node4*>(this);
                n->Destruct();
                return;
            }
            case kNode16:
            {
                const auto n = static_cast<Node16*>(this);
                n->Destruct();
                return;
            }
            case kNode48:
            {
                const auto n = static_cast<Node48*>(this);
                n->Destruct();
                return;
            }
            case kNode256:
            {
                const auto n = static_cast<Node256*>(this);
                n->Destruct();
                return;
            }
        }
    }
}
