#include "node.h"

namespace art_crtp
{
    Node* null_node = nullptr;

    void Node::PrintChild(Node* child, const int i, const int m)
    {
        if (child == nullptr)
            return;

        if (IsLazyExpanded(child))
        {
            const auto val = static_cast<uint32_t>(reinterpret_cast<uint64_t>(child) >> 32);
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

    int Node::CmpLazyExpansion(Node* node_ptr, const uint32_t key)
    {
        // address_value is actual full key value instead of address
        // (key value stored at high 32 bits)
        const uint32_t full_key_value = reinterpret_cast<uint64_t>(node_ptr) >> 32;
        if (key < full_key_value)
            return -1;
        if (key > full_key_value)
            return 1;
        return 0;
    }
}
