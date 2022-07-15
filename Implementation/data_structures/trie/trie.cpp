#include "trie.h"

#include <iostream>

namespace trie
{
    void Trie::Insert(const uint32_t value)
    {
        Node* node = root_;

        for (uint8_t offset = 0; offset < 24; offset += 8)
        {
            // get next 8 bit of value as comparison key
            const uint8_t comparison_key = value >> offset & 0xFF;

            if (node->children_[comparison_key] == nullptr)
            {
                const auto new_node = new Node();

                // create new node on search path
                node->children_[comparison_key] = new_node;

                node = new_node;
            }
            else
            {
                // go to next node
                node = node->children_[comparison_key];
            }
        }

        // insert value at last byte using tagged pointer
        node->children_[value >> 24] = reinterpret_cast<Node*>(static_cast<uint64_t>(value) << 32 | 0x7);
    }

    bool Trie::Find(const uint32_t value) const
    {
        Node* node = root_;

        for (uint8_t offset = 0; offset < 24; offset += 8)
        {
            // get next 8 bit of value as comparison key
            const uint8_t comparison_key = value >> offset & 0xFF;

            if (node->children_[comparison_key] == nullptr)
            {
                // there was no node for this comparison key
                // -> value doesn't exist
                return false;
            }

            // go to next node
            node = node->children_[comparison_key];
        }

        // look if last byte is tagged pointer
        return reinterpret_cast<uint64_t>(node->children_[value >> 24]) & 0x7;
    }

    std::vector<uint32_t> Trie::FindRange(const uint32_t from, const uint32_t to) const
    {
        return std::vector(FindRange(root_, from, to, 0));
    }

    std::vector<uint32_t> Trie::FindRange(Node* node, const uint32_t from, const uint32_t to, const int offset) const
    {
        std::vector<uint32_t> res;

        const uint8_t from_key = from >> offset & 0xFF;
        const uint8_t to_key = to >> offset & 0xFF;

        if (offset == 24)
        {
            for (uint8_t i = from_key; i <= to_key; ++i)
            {
                if (node->children_[i] == nullptr) continue;
                res.push_back(reinterpret_cast<uint64_t>(node->children_[i]) >> 32);
            }
        }
        else
        {
            for (uint8_t i = from_key; i <= to_key; ++i)
            {
                if (node->children_[i] == nullptr) continue;

                auto p = FindRange(node->children_[i], from, to, offset + 8);
                res.insert(res.end(), p.begin(), p.end());
            }
        }

        return res;
    }

    void Trie::Destruct(const Node* node)
    {
        for (const auto& c : node->children_)
        {
            if (c == nullptr || reinterpret_cast<uint64_t>(c) == 0xFFFFFFFFFFFFFFFF) continue;

            Destruct(c);
        }

        delete node;
    }
}
