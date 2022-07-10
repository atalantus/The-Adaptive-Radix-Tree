#pragma once

#include <iostream>
#include <cstdint>

#include "node.h"
#include "node_util.h"
#include "../../../util.h"

namespace art
{
    class Node;

    // null pointer used to indicate non-existing node
    extern Node* null_node;

    enum NodeType : uint8_t
    {
        kNode4, kNode16, kNode48, kNode256
    };

    class Node
    {
    public:
        explicit Node(const NodeType type) : type_{type}, child_count_{0}
        {
        }

        /**
         * Inserts a new partial key with a pointer to a child node into the node and returns the pointer to it.
         * The returned pointer might point to a new node if the node was already full.
         */
        Node* Insert(uint8_t partial_key, Node* child_node);

        /**
         * Finds the child node for a given partial key and returns a reference to the pointer to its memory address.
         *
         * Since this ART uses combined value/pointer slots pointers can also indicate an actual
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
        Node*& FindChild(uint8_t partial_key);

        /**
         * Returns true if the node is full.
         */
        bool IsFull() const;

        /**
         * Destroys this node and its children recursively.
         */
        void Destruct();

        /**
         * Print Tree in preorder way.
         */
        void PrintTree(int depth);

        /**
         * Prints a pointer to a child
         */
        static void PrintChild(Node* child, int i, int m);

        /**
         * Returns true if the pointer value is actually a full key stored using combined value/pointer slots.
         */
        static bool IsLazyExpanded(Node* node_ptr);

        /**
         * Compares a full key stored via lazy expansion at a pointer with key
         * and returns true if they are the same.
         */
        static bool CmpLazyExpansion(Node* node_ptr, uint32_t key);

    protected:
        NodeType type_;
        uint8_t child_count_;
    };

    // ================================================================
    //                      Specific Nodes
    // ================================================================

    class Node4 : public Node
    {
    public:
        Node4() : Node(kNode4), keys_{}, children_{}
        {
        }

        Node* Insert(uint8_t partial_key, Node* child_node);

        Node*& FindChild(uint8_t partial_key);

        void Destruct();

        void PrintTree(int depth) const;

    private:
        uint8_t keys_[4];
        Node* children_[4];
    };

    class Node16 : public Node
    {
    public:
        Node16() : Node(kNode16), keys_{}, children_{}
        {
        }

        Node* Insert(uint8_t partial_key, Node* child_node);

        Node*& FindChild(uint8_t partial_key);

        void Destruct();

        void PrintTree(int depth) const;

    private:
        uint8_t keys_[16];
        Node* children_[16];

        friend class Node4;
    };

    class Node48 : public Node
    {
        static constexpr uint8_t free_marker_ = 48;

    public:
        Node48() : Node(kNode48), keys_{}, children_{}
        {
            std::fill_n(keys_, 256, free_marker_);
        }

        Node* Insert(uint8_t partial_key, Node* child_node);

        Node*& FindChild(uint8_t partial_key);

        void Destruct();

        void PrintTree(int depth) const;

    private:
        uint8_t keys_[256];
        Node* children_[48];

        friend class Node16;
    };

    class Node256 : public Node
    {
    public:
        Node256() : Node(kNode256), children_{}
        {
        }

        Node* Insert(uint8_t partial_key, Node* child_node);

        Node*& FindChild(uint8_t partial_key);

        void Destruct();

        void PrintTree(int depth) const;

    private:
        Node* children_[256];

        friend class Node48;
    };
}
