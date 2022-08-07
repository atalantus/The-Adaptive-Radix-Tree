#pragma once

#include <iostream>
#include <cstdint>
#include <vector>

#include "node.h"
#include "../../../util.h"

namespace art_virt
{
    class Node;

    // null pointer used to indicate non-existing node
    extern Node* null_node;

    /**
     * Abstract base class for different nodes.
     */
    class Node
    {
    public:
        explicit Node() : child_count_{0}
        {
        }

        virtual ~Node() = default;

        /**
         * Inserts a new partial key with a pointer to a child node into the node and returns the pointer to it.
         * The returned pointer might point to a new node if the node was already full.
         */
        virtual Node* Insert(uint8_t partial_key, Node* child_node) = 0;

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
        virtual Node*& FindChild(uint8_t partial_key) = 0;

        /**
         * Recursively finds all values in a given range (inclusive).
         * TODO: Implement GetRange without recursion using custom input iterator.
         */
        virtual std::vector<uint32_t> GetRange(uint32_t from, uint32_t to, int offset) = 0;

        /**
         * Recursively finds all values higher than a given value (inclusive).
         */
        virtual std::vector<uint32_t> GetLowerRange(uint32_t from, int offset) = 0;

        /**
         * Recursively finds all values lower than a given value (inclusive).
         */
        virtual std::vector<uint32_t> GetUpperRange(uint32_t to, int offset) = 0;

        /**
         * Finds all values.
         */
        virtual std::vector<uint32_t> GetFullRange() = 0;

        /**
         * Returns true if the node is full.
         */
        virtual bool IsFull() const = 0;

        /**
         * Destroys this node and its children recursively.
         */
        virtual void Destruct() = 0;

        /**
         * Print Tree in preorder way.
         */
        virtual void PrintTree(int depth) const = 0;

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
         * and returns -1 if the value is less than the stored key, 1 if it's higher and 0 if they're equal.
         */
        static int CmpLazyExpansion(Node* node_ptr, uint32_t key);

    protected:
        uint8_t child_count_;
    };

    // ================================================================
    //                      Specific Nodes
    // ================================================================

    class Node4 : public Node
    {
    public:
        Node4() : Node(), keys_{}, children_{}
        {
        }

        ~Node4() override = default;

        Node* Insert(uint8_t partial_key, Node* child_node) override;

        Node*& FindChild(uint8_t partial_key) override;

        std::vector<uint32_t> GetRange(uint32_t from, uint32_t to, int offset) override;

        std::vector<uint32_t> GetLowerRange(uint32_t from, int offset) override;

        std::vector<uint32_t> GetUpperRange(uint32_t to, int offset) override;

        std::vector<uint32_t> GetFullRange() override;

        bool IsFull() const override;

        void Destruct() override;

        void PrintTree(int depth) const override;

    private:
        uint8_t keys_[4];
        Node* children_[4];
    };

    class Node16 : public Node
    {
    public:
        Node16() : Node(), keys_{}, children_{}
        {
        }

        ~Node16() override = default;

        Node* Insert(uint8_t partial_key, Node* child_node) override;

        Node*& FindChild(uint8_t partial_key) override;

        std::vector<uint32_t> GetRange(uint32_t from, uint32_t to, int offset) override;

        std::vector<uint32_t> GetLowerRange(uint32_t from, int offset) override;

        std::vector<uint32_t> GetUpperRange(uint32_t to, int offset) override;

        std::vector<uint32_t> GetFullRange() override;

        bool IsFull() const override;

        void Destruct() override;

        void PrintTree(int depth) const override;

    private:
        uint8_t keys_[16];
        Node* children_[16];

        friend class Node4;
    };

    class Node48 : public Node
    {
        static constexpr uint8_t free_marker_ = 48;

    public:
        Node48() : Node(), keys_{}, children_{}
        {
            std::fill_n(keys_, 256, free_marker_);
        }

        ~Node48() override = default;

        Node* Insert(uint8_t partial_key, Node* child_node) override;

        Node*& FindChild(uint8_t partial_key) override;

        std::vector<uint32_t> GetRange(uint32_t from, uint32_t to, int offset) override;

        std::vector<uint32_t> GetLowerRange(uint32_t from, int offset) override;

        std::vector<uint32_t> GetUpperRange(uint32_t to, int offset) override;

        std::vector<uint32_t> GetFullRange() override;

        bool IsFull() const override;

        void Destruct() override;

        void PrintTree(int depth) const override;

    private:
        uint8_t keys_[256];
        Node* children_[48];

        friend class Node16;
    };

    class Node256 : public Node
    {
    public:
        Node256() : Node(), children_{}
        {
        }

        ~Node256() override = default;

        Node* Insert(uint8_t partial_key, Node* child_node) override;

        Node*& FindChild(uint8_t partial_key) override;

        std::vector<uint32_t> GetRange(uint32_t from, uint32_t to, int offset) override;

        std::vector<uint32_t> GetLowerRange(uint32_t from, int offset) override;

        std::vector<uint32_t> GetUpperRange(uint32_t to, int offset) override;

        std::vector<uint32_t> GetFullRange() override;

        bool IsFull() const override;

        void Destruct() override;

        void PrintTree(int depth) const override;

    private:
        Node* children_[256];

        friend class Node48;
    };
}
