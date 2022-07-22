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

        void Insert(uint32_t value);

        bool Find(uint32_t value) const;

        std::vector<uint32_t> FindRange(uint32_t from, uint32_t to) const;

    private:
        static void Destruct(Node* node);

        static std::vector<uint32_t> GetRange(Node* node, uint32_t from, uint32_t to, int offset);

        static std::vector<uint32_t> GetLowerRange(Node* node, uint32_t from, int offset);

        static std::vector<uint32_t> GetUpperRange(Node* node, uint32_t to, int offset);

        static std::vector<uint32_t> GetFullRange(Node* node, int offset);

    private:
        Node* root_;
    };
}
