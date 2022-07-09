#include "trie.h"

namespace trie
{
    void Trie::Insert(const uint32_t value)
    {
        Node* node = root_;

        for (uint8_t offset = 0; offset < 32; offset += 8)
        {
            // get next 8 bit of value as comparison key
            const uint8_t comparison_key = (value >> offset) & 0xFF;

            if (node->children_[comparison_key] == nullptr)
            {
                Node* new_node = new Node();

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
    }

    bool Trie::Find(const uint32_t value) const
    {
        Node* node = root_;

        for (uint8_t offset = 0; offset < 32; offset += 8)
        {
            // get next 8 bit of value as comparison key
            const uint8_t comparison_key = (value >> offset) & 0xFF;

            if (node->children_[comparison_key] == nullptr)
            {
                // there was no node for this comparison key
                // -> value doesn't exist
                return false;
            }

            // go to next node
            node = node->children_[comparison_key];
        }

        // compared full value so it exists
        return true;
    }

    std::vector<uint32_t> Trie::FindRange(const uint32_t from, const uint32_t to) const
    {
        return {};
    }

    void Trie::Destruct(const Node* node)
    {
        for (const auto& c: node->children_)
        {
            if (c == nullptr) continue;

            Destruct(c);
        }

        delete node;
    }
}
