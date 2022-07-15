#include "node.h"

#include <cstring>

namespace art
{
    Node* Node4::Insert(const uint8_t partial_key, Node* child_node)
    {
        if (IsFull())
        {
            auto new_node = new Node16();

            memmove(new_node->keys_, keys_, 4);
            memmove(new_node->children_, children_, sizeof(uint64_t) * 4);
            new_node->child_count_ = 4;

            return new_node->Insert(partial_key, child_node);
        }

        // find position to insert new partial key (sorted in ascending order)
        uint8_t pos{0};
        for (; keys_[pos] < partial_key && pos < child_count_; ++pos);

        // move everything from pos
        memmove(keys_ + pos + 1, keys_ + pos, child_count_ - pos);
        memmove(children_ + pos + 1, children_ + pos, (child_count_ - pos) * sizeof(uint64_t));

        // insert
        keys_[pos] = partial_key;
        children_[pos] = child_node;
        ++child_count_;

        return this;
    }

    Node*& Node4::FindChild(const uint8_t partial_key)
    {
        for (uint8_t i = 0; i < child_count_; ++i)
            if (keys_[i] == partial_key)
                return children_[i];

        return null_node;
    }

    void Node4::PrintTree(const int depth) const
    {
        std::cout << "|";
        for (int i = 0; i < depth; ++i)
            std::cout << "-- ";

        std::cout << std::hex << std::uppercase << this << std::dec << " tp:" << +type_ << " cc:" << +child_count_ << " keys{";
        for (int i = 0; i < child_count_; ++i)
        {
            std::cout << std::dec << i << ":" << std::hex << +keys_[i];
            if (i < 3)
                std::cout << ",";
        }
        std::cout << "} children{";
        for (int i = 0; i < child_count_; ++i)
            Node::PrintChild(children_[i], i, child_count_);
        std::cout << "}" << std::endl;

        for (uint8_t i = 0; i < child_count_; ++i)
        {
            if (Node::IsLazyExpanded(children_[i])) continue;
            children_[i]->PrintTree(depth + 1);
        }
    }

    void Node4::Destruct()
    {
        // Destruct children
        for (int i = 0; i < child_count_; ++i)
        {
            if (IsLazyExpanded(children_[i])) continue;
            children_[i]->Destruct();
        }

        // suicide :/
        delete this;
    }
}
