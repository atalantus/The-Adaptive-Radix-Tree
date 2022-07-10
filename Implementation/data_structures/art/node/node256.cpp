#include "node.h"

namespace art
{
    Node* Node256::Insert(const uint8_t partial_key, Node* child_node)
    {
        children_[partial_key] = child_node;
        ++child_count_;
        return this;
    }

    Node*& Node256::FindChild(const uint8_t partial_key)
    {
        return children_[partial_key];
    }

    void Node256::PrintTree(const int depth) const
    {
        std::cout << "|";
        for (int i = 0; i < depth; ++i)
            std::cout << "-- ";

        std::cout << std::hex << std::uppercase << this << std::dec << " tp:" << +type_ << " cc:" << +child_count_ << " children{";
        for (int i = 0; i < 256; ++i)
            Node::PrintChild(children_[i], i, 256);
        std::cout << "}" << std::endl;

        for (int i = 0; i < 256; ++i)
        {
            if (children_[i] == nullptr || Node::IsLazyExpanded(children_[i])) continue;
            children_[i]->PrintTree(depth + 1);
        }
    }

    void Node256::Destruct()
    {
        // Destruct children
        for (auto& i: children_)
        {
            if (i == nullptr || IsLazyExpanded(i)) continue;
            i->Destruct();
        }

        // suicide :/
        delete this;
    }
}
