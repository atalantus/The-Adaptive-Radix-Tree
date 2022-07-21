#include "mtrie.h"

namespace mtrie
{
    void MTrie::Insert(const uint32_t value)
    {
        Node* node = root_;

        for (uint8_t offset = 0; offset < 24; offset += 8)
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
        node->children_.insert({value >> 24, reinterpret_cast<Node*>(static_cast<uint64_t>(value) << 32 | 0x7)});
    }

    bool MTrie::Find(const uint32_t value) const
    {
        Node* node = root_;

        for (uint8_t offset = 0; offset < 24; offset += 8)
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
        return node->children_.contains(value >> 24) && reinterpret_cast<uint64_t>(node->children_[value >> 24]) & 0x7;
    }

    std::vector<uint32_t> MTrie::FindRange(const uint32_t from, const uint32_t to) const
    {
        return FindRange(root_, from, to, 0);
    }

    std::vector<uint32_t> MTrie::FindRange(Node* node, const uint32_t from, const uint32_t to, const int offset) const
    {
        std::vector<uint32_t> res;

        const uint8_t from_key = from >> offset & 0xFF;
        const uint8_t to_key = to >> offset & 0xFF;

        auto it = node->children_.lower_bound(from_key);

        if (it == node->children_.end()) return res;

        if (offset == 24)
        {
            for (; it != node->children_.end() && (*it).first <= to_key; ++it)
            {
                if ((*it).second == nullptr) continue;
                res.push_back(reinterpret_cast<uint64_t>((*it).second) >> 32);
            }
        }
        else
        {
            for (; it != node->children_.end() && (*it).first <= to_key; ++it)
            {
                auto p = FindRange((*it).second, from, to, offset + 8);
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
