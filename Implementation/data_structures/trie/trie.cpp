#include "trie.h"

namespace trie
{
    void Trie::Insert(const uint32_t value)
    {
        Node* node = root_;

        for (int offset = 0; offset < 32; offset += 4)
        {
            // get next 4 bit of value as comparison key
            const uint8_t comparison_key = (value >> offset) & 0xF;

            if (!node->children.contains(comparison_key))
            {
                Node* new_node = new Node();

                // create new node on search path
                node->children.insert({comparison_key, new_node});

                node = new_node;
            } else
            {
                // go to next node
                node = node->children.find(comparison_key)->second;
            }
        }
    }

    bool Trie::Find(const uint32_t value) const
    {
        Node* node = root_;

        for (int offset = 0; offset < 32; offset += 4)
        {
            // get next 4 bit of value as comparison key
            const uint8_t comparison_key = (value >> offset) & 0xF;

            if (!node->children.contains(comparison_key))
            {
                // there was no node for this comparison key
                // -> value doesn't exist
                return false;
            }

            // go to next node
            node = node->children.find(comparison_key)->second;
        }

        // compared full value so it exists
        return true;
    }

    std::vector<uint32_t> Trie::FindRange(const uint32_t from, const uint32_t to) const
    {
        return {};
    }

    void Trie::Destruct(Node* node)
    {
        for (auto& c: node->children)
        {
            Destruct(c.second);
            // TODO: Remove entire hash map entry?
            c.second = nullptr;
        }

        delete node;
    }
}
