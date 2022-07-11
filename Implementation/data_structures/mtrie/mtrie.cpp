#include "mtrie.h"

namespace mtrie
{
    void MTrie::Insert(const uint32_t value)
    {
        Node* node = root_;
        uint8_t offset = 0;

        for (; offset < 24; offset += 8)
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
        node->children_.insert({value >> offset & 0xFF, reinterpret_cast<Node*>(0xFFFFFFFFFFFFFFFF)});
    }

    bool MTrie::Find(const uint32_t value) const
    {
        Node* node = root_;
        uint8_t offset = 0;

        for (; offset < 24; offset += 8)
        {
            // get next 8 bit of value as partial key
            const uint8_t partial_key = value >> offset & 0xFF;

            // check if there is a node
            if (!node->children_.contains(partial_key))
                return false;

            // go to next node
            node = node->children_.find(partial_key)->second;
        }

        // check if last byte exists
        return reinterpret_cast<uint64_t>(node->children_.find(value >> offset & 0xFF)->second) == 0xFFFFFFFFFFFFFFFF;
    }

    std::vector<uint32_t> MTrie::FindRange(const uint32_t from, const uint32_t to) const
    {
        return {};
    }

    void MTrie::Destruct(Node* node)
    {
        for (auto& [_, snd]: node->children_)
        {
            if (reinterpret_cast<uint64_t>(snd) == 0xFFFFFFFFFFFFFFFF) continue;

            Destruct(snd);
            snd = nullptr;
        }

        delete node;
    }
}
