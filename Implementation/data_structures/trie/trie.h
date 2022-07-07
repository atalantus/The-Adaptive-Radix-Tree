#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

namespace trie
{
    class Node
    {
    public:
        Node() : children_(256)
        {
        }

        std::vector<Node*> children_;
    };

    class Trie
    {
    public:
        Trie() : root_{new Node()}
        {
        }

        ~Trie()
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
        void Destruct(const Node* node);

    private:
        Node* root_;
    };
}
