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

        void Insert(uint32_t value);

        bool Find(uint32_t value) const;

        std::vector<uint32_t> FindRange(uint32_t from, uint32_t to) const;

    private:
        static void Destruct(const Node* node);

        static std::vector<uint32_t> GetRange(Node* node, uint32_t from, uint32_t to, int offset);

        static std::vector<uint32_t> GetLowerRange(Node* node, uint32_t from, int offset);

        static std::vector<uint32_t> GetUpperRange(Node* node, uint32_t to, int offset);

        static std::vector<uint32_t> GetFullRange(Node* node, int offset);

    private:
        Node* root_;
    };
}
