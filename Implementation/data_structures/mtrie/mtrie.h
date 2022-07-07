#pragma once

#include <cstdint>
#include <map>
#include <vector>

namespace mtrie
{
    class Node
    {
    public:
        Node() : children_{}
        {
        }

    public:
        std::map<uint8_t, Node*> children_;
    };

    class MTrie
    {
    public:
        MTrie() : root_{new Node()}
        {
        }

        ~MTrie()
        {
            Destruct(root_);
        }

        /**
         * Inserts a 32 bit value into the trie.
         *
         * @param value the value to insert
         */
        void Insert(uint32_t value);

        /**
         * Checks if a 32 bit value exists in the trie.
         *
         * @param value the value to search for
         * @return true if the value exists otherwise false
         */
        bool Find(uint32_t value) const;

        /**
         * Finds all 32 bit values in the trie within a given range.
         *
         * @param from the value to search from (inclusive)
         * @param to the value to search to (inclusive)
         * @return vector of values sorted in ascending order
         */
        std::vector<uint32_t> FindRange(uint32_t from, uint32_t to) const;

    private:
        /**
         * Recursively deletes the complete trie starting at a given node.
         *
         * @param node the root node
         */
        void Destruct(Node* node);

    private:
        Node* root_;
    };
}
