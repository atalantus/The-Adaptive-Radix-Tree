#include "art.h"

#include <functional>

namespace art_virt
{
    void Art::Insert(const uint32_t value)
    {
        std::reference_wrapper<Node*> node_ref = std::ref(root_);

        for (int offset = 24; offset >= 0; offset -= 8)
        {
            // get next 8 bit of value as partial key
            uint8_t partial_key = value >> offset & 0xFF;

            // check if partial key already exists
            Node*& child_node_ref = node_ref.get()->FindChild(partial_key);

            /**
             * Case 1:  Partial key does not exist in the node.
             *          -> Insert full key lazy expanded via combined value/pointer slots.
             */
            if (child_node_ref == null_node)
            {
                const auto tagged_pointer_value = reinterpret_cast<Node*>(static_cast<uint64_t>(value) << 32 | 0x7);

                Node* new_node = node_ref.get()->Insert(partial_key, tagged_pointer_value);

                if (new_node != node_ref.get())
                {
                    // node has changed
                    // -> delete old child and update parent pointer
                    delete node_ref.get();
                    node_ref.get() = new_node;
                }

                return;
            }

            /**
             * Case 2:  Partial key exists and stores a full key (combined value/pointer slots).
             *          -> Either the full key matches or we expand the two different keys until they differ.
             */
            if (Node::IsLazyExpanded(child_node_ref))
            {
                if (Node::CmpLazyExpansion(child_node_ref, value) == 0)
                    // value has already been inserted
                    return;

                // there is already the same partial key for a different full key
                // -> create and add new child nodes until keys differ and then insert them as tagged pointers
                const auto address_value = reinterpret_cast<uint64_t>(child_node_ref);

                const auto new_child_node = new Node4();
                child_node_ref = new_child_node;

                ExpandLazyExpansion(value, address_value >> 32, offset - 8, new_child_node);

                return;
            }

            /**
             * Case 3:  Partial key exists and stores a pointer to a child node.
             *          -> Insert at child node at next depth.
             */
            node_ref = std::ref(child_node_ref);
        }

        __unreachable();
    }

    bool Art::Find(const uint32_t value) const
    {
        Node* node = root_;

        for (int offset = 24; offset >= 0; offset -= 8)
        {
            // get next 8 bit of value as partial key
            const uint8_t partial_key = value >> offset & 0xFF;

            Node* child_node = node->FindChild(partial_key);

            // check if we have a child
            if (child_node == nullptr)
                // since we don't have path compression we know the keys does not exist
                return false;

            // handle lazy expansion
            if (Node::IsLazyExpanded(child_node))
                return Node::CmpLazyExpansion(child_node, value) == 0;

            // go to next node
            node = child_node;
        }

        // compared full value so it exists
        // (since this ART is only for storing 32 bit keys without path compression
        //  or lazy expansion, leaf nodes are not needed)
        return true;
    }

    std::vector<uint32_t> Art::FindRange(const uint32_t from, const uint32_t to) const
    {
        return root_->GetRange(from, to, 24);
    }

    void Art::PrintTree() const
    {
        root_->PrintTree(0);
    }

    void Art::ExpandLazyExpansion(const uint32_t value1, const uint32_t value2, const int depth, Node* node)
    {
        Node* n = node;

        for (int offset = depth; offset >= 0; offset -= 8)
        {
            // get next 8 bit of values as partial keys
            const uint8_t partial_key1 = value1 >> offset & 0xFF;
            const uint8_t partial_key2 = value2 >> offset & 0xFF;

            if (partial_key1 != partial_key2)
            {
                // partial keys differ
                // -> insert both full keys as multi value leaves

                const auto tagged_pointer_value1 = reinterpret_cast<Node*>(static_cast<uint64_t>(value1) << 32 | 0x7);
                const auto tagged_pointer_value2 = reinterpret_cast<Node*>(static_cast<uint64_t>(value2) << 32 | 0x7);

                n->Insert(partial_key1, tagged_pointer_value1);
                n->Insert(partial_key2, tagged_pointer_value2);

                // done
                return;
            }

            // partial keys are still the same
            // -> insert another new node and go to next depth
            const auto new_child_node = new Node4();
            n->Insert(partial_key1, new_child_node);
            n = new_child_node;
        }

        __unreachable();
    }
}
