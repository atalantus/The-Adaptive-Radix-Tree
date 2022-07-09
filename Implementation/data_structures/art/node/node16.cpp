#include "node.h"

#include <cstring>
#include <immintrin.h>

namespace art
{
    Node* Node16::Insert(const uint8_t partial_key, Node* child_node)
    {
        if (IsFull())
        {
            auto new_node = new Node48();

            for (uint8_t i = 0; i < 16; ++i)
            {
                new_node->keys_[keys_[i]] = i;
            }

            memmove(new_node->children_, children_, sizeof(uint64_t) * 16);
            new_node->child_count_ = 16;

            return new_node->Insert(partial_key, child_node);
        }

        // find position to insert new partial key (sorted in ascending order)
        // TODO: What about fragmentation through deletion.
        uint8_t pos{0};
        for (; keys_[pos] < partial_key && pos < child_count_; ++pos);

        // move everything from pos
        memmove(keys_ + pos + 1, keys_ + pos, child_count_ - pos);
        memmove(children_ + pos + 1, children_ + pos, (child_count_ - pos) * sizeof(uint64_t));

        // insert
        keys_[pos] = partial_key;
        children_[pos] = child_node;
        ++child_count_;

        return this;
    }

    Node*& Node16::FindChild(const uint8_t partial_key)
    {
        /**
         * x86-64 SIMD using SSE2 and optionally AVX-512 instructions
         * See for reference: https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html
         */

        // replicate 8 bit partial key to fill 128 bit register
        const __m128i partial_key_set = _mm_set1_epi8(partial_key);
        // store child key set in 128 bit register
        const __m128i child_key_set = _mm_loadu_epi8(keys_);
        // compare partial key set with child key data and store compare bitmask
        // (stores 1 at bit i if keys at position i were equal otherwise 0)
        // using AVX-512 this can be done in one instruction:
        // const __mmask16 cmp_mask = _mm_cmpeq_epi8_mask(partial_key_set, child_key_set);
        const __m128i cmp = _mm_cmpeq_epi8(partial_key_set, child_key_set);
        int cmp_mask = _mm_movemask_epi8(cmp) & (1 << child_count_) - 1;

        if (cmp_mask)
            // return Node pointer in pointer array at index equal to trailing zeros in cmp_mask
            // Note: Would be cool to have an intrinsic for counting trailing zeros in 16 bit integer
            // but I actually couldn't find one that would have worked. (at least in MSVC++)
            // For other compilers this call might have to be substituted with another counting trailing zeros
            // method.
            return children_[__ctz(cmp_mask)];

        return null_node;
    }

    void Node16::PrintTree(int depth) const
    {
        std::cout << "|";
        for (int i = 0; i < depth; ++i)
            std::cout << "--";
        std::cout << " ";

        std::cout << std::hex << std::uppercase << this << " tp:" << +type_ << " cc:" << +child_count_ << " keys{";
        for (int i = 0; i < 16; ++i)
        {
            std::cout << std::dec << i << ":" << std::hex << +keys_[i];
            if (i < 15)
                std::cout << ",";
        }
        std::cout << "} children{";
        for (int i = 0; i < 16; ++i)
        {
            Node::PrintChild(children_[i], i);
            if (i < 15)
                std::cout << ",";
        }
        std::cout << "}" << std::endl;

        for (uint8_t i = 0; i < child_count_; ++i)
        {
            if (Node::IsLazyExpanded(reinterpret_cast<uint64_t>(children_[i]))) continue;
            children_[i]->PrintTree(depth + 1);
        }
    }
}
