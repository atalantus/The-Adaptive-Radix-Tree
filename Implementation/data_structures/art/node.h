#pragma once

#include <cstdint>

namespace art
{
    enum NodeType : uint8_t { kNode4, kNode16, kNode48, kNode256 };

    class Node
    {
    public:
        explicit Node(const NodeType type) : type_{type}, child_count_{0}, key_data_{nullptr}, pointer_data_(nullptr)
        {
        }

        explicit Node(const NodeType type, const uint8_t child_count) : type_{type}, child_count_{child_count},
                                                                        key_data_{nullptr}, pointer_data_(nullptr)
        {
        }

        /**
         * Finds the child node for a given partial key and returns the nodes address.
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
        uint64_t FindChild(const uint8_t partial_key);

        bool IsFull();
        bool IsUnderfull();

    private:
        NodeType type_;
        // number of non null children
        uint8_t child_count_;
        // pointer to key/index data array
        uint8_t* key_data_;
        // pointer to child pointer array
        uint64_t* pointer_data_;
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
