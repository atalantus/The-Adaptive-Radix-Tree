#include "node.h"

#include <cstring>

namespace art_exp
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

    std::vector<uint32_t> Node4::GetRange(const uint32_t from, const uint32_t to, const int offset) const
    {
        std::vector<uint32_t> res;

        const uint8_t from_key = from >> offset & 0xFF;
        const uint8_t to_key = to >> offset & 0xFF;

        uint8_t i = 0;
        for (; i < child_count_ && keys_[i] < from_key; ++i);

        if (i == child_count_ || keys_[i] > to_key) return res;

        if (from_key != to_key)
        {
            if (keys_[i] == from_key)
            {
                if (!IsLazyExpanded(children_[i]))
                {
                    auto p = children_[i]->GetLowerRange(from, offset - 8);
                    res.insert(res.end(), p.begin(), p.end());
                }
                else if (CmpLazyExpansion(children_[i], from) <= 0)
                    res.push_back(reinterpret_cast<uint64_t>(children_[i]) >> 32);
                ++i;
            }

            for (; i < child_count_ && keys_[i] < to_key; ++i)
            {
                if (!IsLazyExpanded(children_[i]))
                {
                    auto p = children_[i]->GetFullRange();
                    res.insert(res.end(), p.begin(), p.end());
                }
                else
                    res.push_back(reinterpret_cast<uint64_t>(children_[i]) >> 32);
            }

            if (i < child_count_ && keys_[i] == to_key)
            {
                if (!IsLazyExpanded(children_[i]))
                {
                    auto p = children_[i]->GetUpperRange(to, offset - 8);
                    res.insert(res.end(), p.begin(), p.end());
                }
                else if (CmpLazyExpansion(children_[i], to) >= 0)
                    res.push_back(reinterpret_cast<uint64_t>(children_[i]) >> 32);
            }
        }
        else
        {
            if (!IsLazyExpanded(children_[i]))
                return children_[i]->GetRange(from, to, offset - 8);

            if (CmpLazyExpansion(children_[i], from) <= 0 && CmpLazyExpansion(children_[i], to) >= 0)
                res.push_back(reinterpret_cast<uint64_t>(children_[i]) >> 32);
        }

        return res;
    }

    std::vector<uint32_t> Node4::GetLowerRange(const uint32_t from, const int offset) const
    {
        std::vector<uint32_t> res;

        const uint8_t from_key = from >> offset & 0xFF;

        uint8_t i = 0;
        for (; i < child_count_ && keys_[i] < from_key; ++i);

        if (i == child_count_) return res;

        if (keys_[i] == from_key)
        {
            if (!IsLazyExpanded(children_[i]))
            {
                auto p = children_[i]->GetLowerRange(from, offset - 8);
                res.insert(res.end(), p.begin(), p.end());
            }
            else if (CmpLazyExpansion(children_[i], from) <= 0)
                res.push_back(reinterpret_cast<uint64_t>(children_[i]) >> 32);
            ++i;
        }

        for (; i < child_count_; ++i)
        {
            if (!IsLazyExpanded(children_[i]))
            {
                auto p = children_[i]->GetFullRange();
                res.insert(res.end(), p.begin(), p.end());
            }
            else
                res.push_back(reinterpret_cast<uint64_t>(children_[i]) >> 32);
        }

        return res;
    }

    std::vector<uint32_t> Node4::GetUpperRange(const uint32_t to, const int offset) const
    {
        std::vector<uint32_t> res;

        const uint8_t to_key = to >> offset & 0xFF;

        uint8_t i = 0;

        if (i == child_count_ || keys_[i] > to_key) return res;

        for (; i < child_count_ && keys_[i] < to_key; ++i)
        {
            if (!IsLazyExpanded(children_[i]))
            {
                auto p = children_[i]->GetFullRange();
                res.insert(res.end(), p.begin(), p.end());
            }
            else
                res.push_back(reinterpret_cast<uint64_t>(children_[i]) >> 32);
        }

        if (i < child_count_ && keys_[i] == to_key)
        {
            if (!IsLazyExpanded(children_[i]))
            {
                auto p = children_[i]->GetUpperRange(to, offset - 8);
                res.insert(res.end(), p.begin(), p.end());
            }
            else if (CmpLazyExpansion(children_[i], to) >= 0)
                res.push_back(reinterpret_cast<uint64_t>(children_[i]) >> 32);
        }

        return res;
    }

    std::vector<uint32_t> Node4::GetFullRange() const
    {
        std::vector<uint32_t> res;

        for (uint8_t i = 0; i < child_count_; ++i)
        {
            if (IsLazyExpanded(children_[i]))
                res.push_back(reinterpret_cast<uint64_t>(children_[i]) >> 32);
            else
            {
                auto p = children_[i]->GetFullRange();
                res.insert(res.end(), p.begin(), p.end());
            }
        }

        return res;
    }

    void Node4::PrintTree(const int depth) const
    {
        std::cout << "|";
        for (int i = 0; i < depth; ++i)
            std::cout << "-- ";

        std::cout << std::hex << std::uppercase << this << std::dec << " tp:" << +type_ << " cc:" << +child_count_ <<
            " keys{";
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
