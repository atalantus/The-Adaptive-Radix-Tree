#include "trie.h"

#include <iostream>

namespace trie
{
    void Trie::Insert(const uint32_t value)
    {
        Node* node = root_;
        uint8_t offset = 0;

        for (; offset < 24; offset += 8)
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

        // insert last byte
        node->children_[value >> offset & 0xFF] = reinterpret_cast<Node*>(0xFFFFFFFFFFFFFFFF);
    }

    bool Trie::Find(const uint32_t value) const
    {
        Node* node = root_;
        uint8_t offset = 0;

        for (; offset < 24; offset += 8)
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

        // look if last byte is set
        return reinterpret_cast<uint64_t>(node->children_[value >> offset & 0xFF]) == 0xFFFFFFFFFFFFFFFF;
    }

    std::vector<uint32_t> Trie::FindRange(const uint32_t from, const uint32_t to) const
    {
        return {};
    }

    void Trie::Destruct(const Node* node)
    {
        for (const auto& c: node->children_)
        {
            if (c == nullptr || reinterpret_cast<uint64_t>(c) == 0xFFFFFFFFFFFFFFFF) continue;

            Destruct(c);
        }

        delete node;
    }
}
