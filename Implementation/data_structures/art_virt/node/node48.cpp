#include "node.h"

namespace art_virt
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

        // find next free index
        int free_index = child_count_;
        if (children_[free_index] != nullptr)
            for (free_index = 0; free_index < 48 && children_[free_index] != nullptr; ++free_index);

        keys_[partial_key] = free_index;
        children_[free_index] = child_node;
        ++child_count_;

        return this;
    }

    Node*& Node48::FindChild(const uint8_t partial_key)
    {
        if (keys_[partial_key] != free_marker_)
            return children_[keys_[partial_key]];

        return null_node;
    }

    std::vector<uint32_t> Node48::GetRange(const uint32_t from, const uint32_t to, const int offset)
    {
        std::vector<uint32_t> res;

        const uint8_t from_key = from >> offset & 0xFF;
        const uint8_t to_key = to >> offset & 0xFF;

        if (from_key != to_key)
        {
            if (keys_[from_key] != free_marker_)
            {
                if (!IsLazyExpanded(children_[keys_[from_key]]))
                {
                    auto p = children_[keys_[from_key]]->GetLowerRange(from, offset - 8);
                    res.insert(res.end(), p.begin(), p.end());
                }
                else if (CmpLazyExpansion(children_[keys_[from_key]], from) <= 0)
                    res.push_back(reinterpret_cast<uint64_t>(children_[keys_[from_key]]) >> 32);
            }

            for (uint16_t i = from_key + 1; i < to_key; ++i)
            {
                if (keys_[i] == free_marker_) continue;

                if (!IsLazyExpanded(children_[keys_[i]]))
                {
                    auto p = children_[keys_[i]]->GetFullRange();
                    res.insert(res.end(), p.begin(), p.end());
                }
                else
                    res.push_back(reinterpret_cast<uint64_t>(children_[keys_[i]]) >> 32);
            }

            if (keys_[to_key] != free_marker_)
            {
                if (!IsLazyExpanded(children_[keys_[to_key]]))
                {
                    auto p = children_[keys_[to_key]]->GetUpperRange(to, offset - 8);
                    res.insert(res.end(), p.begin(), p.end());
                }
                else if (CmpLazyExpansion(children_[keys_[to_key]], to) >= 0)
                    res.push_back(reinterpret_cast<uint64_t>(children_[keys_[to_key]]) >> 32);
            }
        }
        else
        {
            if (keys_[from_key] == free_marker_) return res;

            if (!IsLazyExpanded(children_[keys_[from_key]]))
                return children_[keys_[from_key]]->GetRange(from, to, offset - 8);

            if (CmpLazyExpansion(children_[keys_[from_key]], from) <= 0 && CmpLazyExpansion(children_[keys_[from_key]], to) >= 0)
                res.push_back(reinterpret_cast<uint64_t>(children_[keys_[from_key]]) >> 32);
        }

        return res;
    }

    std::vector<uint32_t> Node48::GetLowerRange(const uint32_t from, const int offset)
    {
        std::vector<uint32_t> res;

        const uint8_t from_key = from >> offset & 0xFF;

        if (keys_[from_key] != free_marker_)
        {
            if (!IsLazyExpanded(children_[keys_[from_key]]))
            {
                auto p = children_[keys_[from_key]]->GetLowerRange(from, offset - 8);
                res.insert(res.end(), p.begin(), p.end());
            }
            else if (CmpLazyExpansion(children_[keys_[from_key]], from) <= 0)
                res.push_back(reinterpret_cast<uint64_t>(children_[keys_[from_key]]) >> 32);
        }

        for (uint16_t i = from_key + 1; i < 256; ++i)
        {
            if (keys_[i] == free_marker_) continue;

            if (!IsLazyExpanded(children_[keys_[i]]))
            {
                auto p = children_[keys_[i]]->GetFullRange();
                res.insert(res.end(), p.begin(), p.end());
            }
            else
                res.push_back(reinterpret_cast<uint64_t>(children_[keys_[i]]) >> 32);
        }

        return res;
    }

    std::vector<uint32_t> Node48::GetUpperRange(const uint32_t to, const int offset)
    {
        std::vector<uint32_t> res;

        const uint8_t to_key = to >> offset & 0xFF;

        for (uint16_t i = 0; i < to_key; ++i)
        {
            if (keys_[i] == free_marker_) continue;

            if (!IsLazyExpanded(children_[keys_[i]]))
            {
                auto p = children_[keys_[i]]->GetFullRange();
                res.insert(res.end(), p.begin(), p.end());
            }
            else
                res.push_back(reinterpret_cast<uint64_t>(children_[keys_[i]]) >> 32);
        }

        if (keys_[to_key] != free_marker_)
        {
            if (!IsLazyExpanded(children_[keys_[to_key]]))
            {
                auto p = children_[keys_[to_key]]->GetUpperRange(to, offset - 8);
                res.insert(res.end(), p.begin(), p.end());
            }
            else if (CmpLazyExpansion(children_[keys_[to_key]], to) >= 0)
                res.push_back(reinterpret_cast<uint64_t>(children_[keys_[to_key]]) >> 32);
        }

        return res;
    }

    std::vector<uint32_t> Node48::GetFullRange()
    {
        std::vector<uint32_t> res;

        for (uint16_t i = 0; i < 256; ++i)
        {
            if (keys_[i] == free_marker_) continue;

            if (!IsLazyExpanded(children_[keys_[i]]))
            {
                auto p = children_[keys_[i]]->GetFullRange();
                res.insert(res.end(), p.begin(), p.end());
            }
            else
                res.push_back(reinterpret_cast<uint64_t>(children_[keys_[i]]) >> 32);
        }

        return res;
    }

    void Node48::PrintTree(const int depth) const
    {
        std::cout << "|";
        for (int i = 0; i < depth; ++i)
            std::cout << "-- ";

        std::cout << std::hex << std::uppercase << this << std::dec << " tp:" << 2 << " cc:" << +child_count_ <<
            " keys{";
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
                if (Node::IsLazyExpanded(children_[keys_[i]])) continue;
                children_[keys_[i]]->PrintTree(depth + 1);
            }
        }
    }

        bool Node48::IsFull() const
    {
        return child_count_ == 48;
    }

    void Node48::Destruct()
    {
        // Destruct children
        for (auto& i : children_)
        {
            if (i == nullptr || IsLazyExpanded(i)) continue;
            i->Destruct();
        }

        // suicide :/
        delete this;
    }
}
