#include "node.h"

#include <cstring>
#include <immintrin.h>

namespace art
{
    Node* Node16::Insert(const uint8_t partial_key, Node* child_node)
    {
        if (IsFull())
        {
            const auto new_node = new Node48();

            for (uint8_t i = 0; i < 16; ++i)
            {
                new_node->keys_[keys_[i]] = i;
            }

            memmove(new_node->children_, children_, sizeof(uint64_t) * 16);
            new_node->child_count_ = 16;

            return new_node->Insert(partial_key, child_node);
        }

        // find position to insert new partial key (sorted in ascending order)

        /**
         * x86-64 SIMD using SSE2
         * See for reference: https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html
         */
        // flip sign bit for signed SIMD comparison
        const int8_t partial_key_flipped = partial_key ^ 128;
        // for exact documentation see Node16::FindChild below
        const __m128i partial_key_set = _mm_set1_epi8(partial_key_flipped);
        const __m128i child_key_set = _mm_loadu_si128(reinterpret_cast<__m128i*>(keys_));
        // compare less than
        const __m128i cmp = _mm_cmplt_epi8(partial_key_set, child_key_set);
        const int cmp_mask = _mm_movemask_epi8(cmp) & ((1 << child_count_) - 1);
        const uint32_t pos = cmp_mask ? __ctz(cmp_mask) : child_count_;

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
        // use _mm_loadu_si128 instead of _mm_loadu_epi8 for not needing AVX-512
        const __m128i child_key_set = _mm_loadu_si128(reinterpret_cast<__m128i*>(keys_));
        // compare partial key set with child key data and store compare bitmask
        // (stores 1 at bit i if keys at position i were equal otherwise 0)
        // using AVX-512 this can be done in one instruction:
        // const __mmask16 cmp_mask = _mm_cmpeq_epi8_mask(partial_key_set, child_key_set);
        const __m128i cmp = _mm_cmpeq_epi8(partial_key_set, child_key_set);
        // only use mask up to child_count_ (needed when searching 0th partial key since unused key elements are also 0)
        const int cmp_mask = _mm_movemask_epi8(cmp) & ((1 << child_count_) - 1);

        if (cmp_mask)
            // return Node pointer in pointer array at index equal to trailing zeros in cmp_mask
            // Note: Would be cool to have an intrinsic for counting trailing zeros in 16 bit integer
            // but I actually couldn't find one that would have worked. (at least in MSVC++)
            // For other compilers this call might have to be substituted with another counting trailing zeros
            // method.
            return children_[__ctz(cmp_mask)];

        return null_node;
    }

    std::vector<uint32_t> Node16::FindRange(const uint32_t from, const uint32_t to, const int offset)
    {
        std::vector<uint32_t> res;

        const uint8_t from_key = from >> offset & 0xFF;
        const uint8_t to_key = to >> offset & 0xFF;

        const __m128i cmp = _mm_cmplt_epi8(
            _mm_set1_epi8(from_key ^ 128), 
            _mm_loadu_si128(reinterpret_cast<__m128i*>(keys_))
        );
        const int cmp_mask = _mm_movemask_epi8(cmp) & ((1 << child_count_) - 1);

        if (!cmp_mask) return res;

        for (uint16_t i = __ctz(cmp_mask); i < child_count_ && keys_[i] <= to_key; ++i)
        {
            if (IsLazyExpanded(children_[i]))
                res.push_back(reinterpret_cast<uint64_t>(children_[i]) >> 32);
            else
            {
                auto p = children_[i]->FindRange(from, to, offset + 8);
                res.insert(res.end(), p.begin(), p.end());
            }
        }

        return res;
    }

    void Node16::PrintTree(const int depth) const
    {
        std::cout << "|";
        for (int i = 0; i < depth; ++i)
            std::cout << "-- ";

        std::cout << std::hex << std::uppercase << this << std::dec << " tp:" << +type_ << " cc:" << +child_count_ <<
            " keys{";
        for (int i = 0; i < 16; ++i)
        {
            if (keys_[i] == 0 && children_[i] == nullptr) continue;
            std::cout << std::dec << i << ":" << std::hex << +keys_[i];
            if (i < 15)
                std::cout << ",";
        }
        std::cout << "} children{";
        for (int i = 0; i < 16; ++i)
            Node::PrintChild(children_[i], i, 16);
        std::cout << "}" << std::endl;

        for (uint8_t i = 0; i < child_count_; ++i)
        {
            if (Node::IsLazyExpanded(children_[i])) continue;
            children_[i]->PrintTree(depth + 1);
        }
    }

    void Node16::Destruct()
    {
        // Destruct children
        for (int i = 0; i < child_count_; ++i)
        {
            if (IsLazyExpanded(children_[i])) continue;
            children_[i]->Destruct();
        }

        // suicide :/
        delete this;
    }
}
