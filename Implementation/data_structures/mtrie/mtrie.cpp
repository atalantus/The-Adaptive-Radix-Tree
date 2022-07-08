#include "mtrie.h"

namespace mtrie
{
    void MTrie::Insert(const uint32_t value)
    {
        Node* node = root_;

        for (int offset = 0; offset < 32; offset += 8)
        {
            // get next 8 bit of value as comparison key
            const uint8_t comparison_key = (value >> offset) & 0xFF;

            if (!node->children_.contains(comparison_key))
            {
                Node* new_node = new Node();

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
    }

    bool MTrie::Find(const uint32_t value) const
    {
        Node* node = root_;

        for (uint8_t offset = 0; offset < 32; offset += 8)
        {
            // get next 8 bit of value as partial key
            const uint8_t partial_key = (value >> offset) & 0xFF;

            // check if there is a node
            if (!node->children_.contains(partial_key))
                return false;


            // go to next node
            node = node->children_.find(partial_key)->second;
        }

        // compared full value so it exists
        return true;
    }

    std::vector<uint32_t> MTrie::FindRange(const uint32_t from, const uint32_t to) const
    {
        return {};
    }

    void MTrie::Destruct(Node* node)
    {
        for (auto& c : node->children_)
        {
            Destruct(c.second);
            c.second = nullptr;
        }

        delete node;
    }
}
