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

    Node*& Node48::FindChild(const uint8_t partial_key)
    {
        if (keys_[partial_key] != free_marker_)
            return children_[keys_[partial_key]];

        return null_node;
    }

    void Node48::PrintTree(int depth) const
    {
        std::cout << "|";
        for (int i = 0; i < depth; ++i)
            std::cout << "--";
        std::cout << " ";

        std::cout << std::hex << std::uppercase << this << std::dec << " tp:" << +type_ << " cc:" << +child_count_ << " keys{";
        for (int i = 0; i < 256; ++i)
        {
            if (keys_[i] == free_marker_) continue;
            std::cout << std::hex << i << ":" << std::dec << +keys_[i];
            if (i < 255)
                std::cout << ",";
        }
        std::cout << "} children{";
        for (int i = 0; i < 48; ++i)
            Node::PrintChild(children_[i], i, 48);
        std::cout << "}" << std::endl;

        for (int i = 0; i < 256; ++i)
        {
            if (keys_[i] != free_marker_)
            {
                if (Node::IsLazyExpanded(reinterpret_cast<uint64_t>(children_[keys_[i]]))) continue;
                children_[keys_[i]]->PrintTree(depth + 1);
            }
        }
    }
}
