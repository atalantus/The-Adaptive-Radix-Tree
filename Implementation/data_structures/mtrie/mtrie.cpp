#include "mtrie.h"

namespace mtrie
{
    void MTrie::Insert(const uint32_t value)
    {
        Node* node = root_;

        for (uint8_t offset = 24; offset > 0; offset -= 8)
        {
            // get next 8 bit of value as comparison key
            const uint8_t comparison_key = value >> offset & 0xFF;

            if (!node->children_.contains(comparison_key))
            {
                auto new_node = new Node();

                // create new node on search path
                node->children_.insert({comparison_key, new_node});

                node = new_node;
            }
            else
            {
                // go to next node
                node = node->children_.find(comparison_key)->second;
            }
        }

        // insert last byte
        node->children_.insert({value & 0xFF, reinterpret_cast<Node*>(static_cast<uint64_t>(value) << 32 | 0x7)});
    }

    bool MTrie::Find(const uint32_t value) const
    {
        Node* node = root_;

        for (uint8_t offset = 24; offset > 0; offset -= 8)
        {
            // get next 8 bit of value as partial key
            const uint8_t partial_key = value >> offset & 0xFF;

            // check if there is a node
            if (!node->children_.contains(partial_key))
                return false;

            // go to next node
            node = node->children_.find(partial_key)->second;
        }

        // look if last byte is tagged pointer
        return node->children_.contains(value & 0xFF) && reinterpret_cast<uint64_t>(node->children_.at(value & 0xFF)) & 0x7;
    }

    std::vector<uint32_t> MTrie::FindRange(const uint32_t from, const uint32_t to) const
    {
        return GetRange(root_, from, to, 24);
    }

    std::vector<uint32_t> MTrie::GetRange(Node* node, const uint32_t from, const uint32_t to, const int offset)
    {
        std::vector<uint32_t> res;

        const uint8_t from_key = from >> offset & 0xFF;
        const uint8_t to_key = to >> offset & 0xFF;

        if (offset == 0)
        {
            for (auto it = node->children_.lower_bound(from_key); it != node->children_.end() && it->first <= to_key; ++it)
                res.push_back(reinterpret_cast<uint64_t>(it->second) >> 32);
        }
        else if (from_key != to_key)
        {
            auto it = node->children_.lower_bound(from_key);

            if (it == node->children_.end()) return res;

            if (it->first == from_key)
            {
                auto p = GetLowerRange(node->children_.at(from_key), from, offset - 8);
                res.insert(res.end(), p.begin(), p.end());
                ++it;
            }

            for (; it != node->children_.end() && it->first < to_key; ++it)
            {
                auto p = GetFullRange(it->second, offset - 8);
                res.insert(res.end(), p.begin(), p.end());
            }

            if (it != node->children_.end() && it->first == to_key)
            {
                auto p = GetUpperRange(node->children_.at(to_key), to, offset - 8);
                res.insert(res.end(), p.begin(), p.end());
            }
        }
        else
        {
            if (node->children_.contains(from_key))
                return GetRange(node->children_.at(from_key), from, to, offset - 8);
        }

        return res;
    }

    std::vector<uint32_t> MTrie::GetLowerRange(Node* node, const uint32_t from, const int offset)
    {
        std::vector<uint32_t> res;

        const uint8_t from_key = from >> offset & 0xFF;

        if (offset == 0)
        {
            for (auto it = node->children_.lower_bound(from_key); it != node->children_.end(); ++it)
                res.push_back(reinterpret_cast<uint64_t>(it->second) >> 32);
        }
        else
        {
            auto it = node->children_.lower_bound(from_key);

            if (it == node->children_.end()) return res;

            if (it->first == from_key)
            {
                auto p = GetLowerRange(node->children_.at(from_key), from, offset - 8);
                res.insert(res.end(), p.begin(), p.end());
                ++it;
            }

            for (; it != node->children_.end(); ++it)
            {
                auto p = GetFullRange(it->second, offset - 8);
                res.insert(res.end(), p.begin(), p.end());
            }
        }

        return res;
    }

    std::vector<uint32_t> MTrie::GetUpperRange(Node* node, const uint32_t to, const int offset)
    {
        std::vector<uint32_t> res;

        const uint8_t to_key = to >> offset & 0xFF;

        if (offset == 0)
        {
            for (auto it = node->children_.begin(); it != node->children_.end() && it->first <= to_key; ++it)
                res.push_back(reinterpret_cast<uint64_t>(it->second) >> 32);
        }
        else
        {
            auto it = node->children_.begin();

            if (it == node->children_.end()) return res;

            for (; it != node->children_.end() && it->first < to_key; ++it)
            {
                auto p = GetFullRange(it->second, offset - 8);
                res.insert(res.end(), p.begin(), p.end());
            }

            if (it != node->children_.end() && it->first == to_key)
            {
                auto p = GetUpperRange(node->children_.at(to_key), to, offset - 8);
                res.insert(res.end(), p.begin(), p.end());
            }
        }

        return res;
    }

    std::vector<uint32_t> MTrie::GetFullRange(Node* node, const int offset)
    {
        std::vector<uint32_t> res;

        if (offset == 0)
        {
            for (auto it = node->children_.begin(); it != node->children_.end(); ++it)
                res.push_back(reinterpret_cast<uint64_t>(it->second) >> 32);
        }
        else
        {
            for (auto it = node->children_.begin(); it != node->children_.end(); ++it)
            {
                auto p = GetFullRange(it->second, offset - 8);
                res.insert(res.end(), p.begin(), p.end());
            }
        }

        return res;
    }

    void MTrie::Destruct(Node* node)
    {
        for (auto& [_, snd] : node->children_)
        {
            if (reinterpret_cast<uint64_t>(snd) & 0x7) continue;

            Destruct(snd);
            snd = nullptr;
        }

        delete node;
    }
}
