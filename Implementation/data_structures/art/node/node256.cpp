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

    std::vector<uint32_t> Node256::GetRange(const uint32_t from, const uint32_t to, const int offset) const
    {
        std::vector<uint32_t> res;

        const uint8_t from_key = from >> offset & 0xFF;
        const uint8_t to_key = to >> offset & 0xFF;

        if (from_key != to_key)
        {
            if (children_[from_key] != nullptr)
            {
                if (!IsLazyExpanded(children_[from_key]))
                {
                    auto p = children_[from_key]->GetLowerRange(from, offset - 8);
                    res.insert(res.end(), p.begin(), p.end());
                }
                else if (CmpLazyExpansion(children_[from_key], from) <= 0)
                    res.push_back(reinterpret_cast<uint64_t>(children_[from_key]) >> 32);
            }

            for (uint16_t i = from_key + 1; i < to_key; ++i)
            {
                if (children_[i] == nullptr) continue;

                if (!IsLazyExpanded(children_[i]))
                {
                    auto p = children_[i]->GetFullRange();
                    res.insert(res.end(), p.begin(), p.end());
                }
                else
                    res.push_back(reinterpret_cast<uint64_t>(children_[i]) >> 32);
            }

            if (children_[to_key] != nullptr)
            {
                if (!IsLazyExpanded(children_[to_key]))
                {
                    auto p = children_[to_key]->GetUpperRange(to, offset - 8);
                    res.insert(res.end(), p.begin(), p.end());
                }
                else if (CmpLazyExpansion(children_[to_key], to) >= 0)
                    res.push_back(reinterpret_cast<uint64_t>(children_[to_key]) >> 32);
            }
        }
        else
        {
            if (children_[from_key] == nullptr) return res;

            if (!IsLazyExpanded(children_[to_key]))
                return children_[from_key]->GetRange(from, to, offset - 8);

            if (CmpLazyExpansion(children_[from_key], from) <= 0 && CmpLazyExpansion(children_[from_key], to) >= 0)
                res.push_back(reinterpret_cast<uint64_t>(children_[to_key]) >> 32);
        }

        return res;
    }

    std::vector<uint32_t> Node256::GetLowerRange(const uint32_t from, const int offset) const
    {
        std::vector<uint32_t> res;

        const uint8_t from_key = from >> offset & 0xFF;

        if (children_[from_key] != nullptr)
        {
            if (!IsLazyExpanded(children_[from_key]))
            {
                auto p = children_[from_key]->GetLowerRange(from, offset - 8);
                res.insert(res.end(), p.begin(), p.end());
            }
            else if (CmpLazyExpansion(children_[from_key], from) <= 0)
                res.push_back(reinterpret_cast<uint64_t>(children_[from_key]) >> 32);
        }

        for (uint16_t i = from_key + 1; i < 256; ++i)
        {
            if (children_[i] == nullptr) continue;

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

    std::vector<uint32_t> Node256::GetUpperRange(const uint32_t to, const int offset) const
    {
        std::vector<uint32_t> res;

        const uint8_t to_key = to >> offset & 0xFF;

        for (uint16_t i = 0; i < to_key; ++i)
        {
            if (children_[i] == nullptr) continue;

            if (!IsLazyExpanded(children_[i]))
            {
                auto p = children_[i]->GetFullRange();
                res.insert(res.end(), p.begin(), p.end());
            }
            else
                res.push_back(reinterpret_cast<uint64_t>(children_[i]) >> 32);
        }

        if (children_[to_key] != nullptr)
        {
            if (!IsLazyExpanded(children_[to_key]))
            {
                auto p = children_[to_key]->GetUpperRange(to, offset - 8);
                res.insert(res.end(), p.begin(), p.end());
            }
            else if (CmpLazyExpansion(children_[to_key], to) >= 0)
                res.push_back(reinterpret_cast<uint64_t>(children_[to_key]) >> 32);
        }

        return res;
    }

    std::vector<uint32_t> Node256::GetFullRange() const
    {
        std::vector<uint32_t> res;

        for (uint16_t i = 0; i < 256; ++i)
        {
            if (children_[i] == nullptr) continue;

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

    void Node256::PrintTree(const int depth) const
    {
        std::cout << "|";
        for (int i = 0; i < depth; ++i)
            std::cout << "-- ";

        std::cout << std::hex << std::uppercase << this << std::dec << " tp:" << +type_ << " cc:" << +child_count_ <<
            " children{";
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
        for (auto& i : children_)
        {
            if (i == nullptr || IsLazyExpanded(i)) continue;
            i->Destruct();
        }

        // suicide :/
        delete this;
    }
}
