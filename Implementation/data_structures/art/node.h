#pragma once

#include <cstdint>

namespace art
{
    enum NodeType : uint8_t { kNode4, kNode16, kNode48, kNode256 };

    class Node
    {
    public:
        Node() : type_{kNode4}, child_count_{0}, data_{new uint8_t[36]}
        {
        }

        ~Node()
        {
            Destruct();
        }

        /**
         * Finds the child node for a given partial key and its memory address.
         *
         * Since this ART uses multi-value leaves addresses can also indicate an actual
         * 32 bit value.
         *
         * The method used for this implementation is pointer tagging.
         *
         * For x86-64 we use the low 3 bits to indicate that the 64 bit return value is
         * stores in fact an actual full key value instead of a memory address.
         *
         * As on 64 bit machines the addresses are 8-byte aligned the low 3 bits will always be 0
         * for an actual address. The low 3 bits being 1 indicates the high 32 bits storing an 32 key
         * value.
         */
        uint64_t FindChild(uint8_t partial_key);

        /**
         * Inserts a new partial key, address value pair into the node.
         */
        void Insert(uint8_t partial_key, uint64_t address_value);


    private:
        /**
         * Expands a node to the next bigger node type.
         */
        void Expand();

        /**
         * Recursively deletes the nodes.
         */
        void Destruct();

        bool IsFull();
        bool IsUnderfull();

    private:
        // this nodes type
        NodeType type_;
        // number of non null children
        uint8_t child_count_;
        uint8_t* data_;
    };

    inline bool Node::IsFull()
    {
        switch (type_)
        {
            case kNode4:
                return child_count_ == 4;
            case kNode16:
                return child_count_ == 16;
            case kNode48:
                return child_count_ == 48;
            case kNode256:
                return false;
        }
    }

    inline bool Node::IsUnderfull()
    {
        switch (type_)
        {
            case kNode4:
                return false;
            case kNode16:
                return child_count_ < 5;
            case kNode48:
                return child_count_ < 17;
            case kNode256:
                return child_count_ < 49;
        }
    }
}
