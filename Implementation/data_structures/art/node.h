#pragma once

#include <cstdint>

namespace art
{
    enum NodeType : uint8_t { kNode4, kNode16, kNode48, kNode256 };

    class Node
    {
    public:
        explicit Node(const NodeType type) : type_{type}, child_count_{0}, data_{nullptr}
        {
        }

        explicit Node(const NodeType type, const uint8_t child_count) : type_{type}, child_count_{child_count},
                                                                        data_{nullptr}
        {
        }

        Node* FindChild(const uint8_t partial_key);

        bool IsFull();
        bool IsUnderfull();

    private:
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
