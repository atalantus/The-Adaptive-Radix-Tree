#include "node.h"

// MSVC++ headers
// might be different for other compilers
// Note: >= GCC 11 required (https://gcc.gnu.org/bugzilla/show_bug.cgi?id=95483)
#include <intrin.h>
#include <immintrin.h>

namespace art
{
    uint64_t Node::FindChild(const uint8_t partial_key)
    {
        if (type_ == kNode4)
        {
            for (uint8_t i = 0; i < 4; ++i)
                if (data_[i] == partial_key)
                    return *reinterpret_cast<uint64_t*>(&data_[i * 8 + 4]);

            return NULL;
        }

        if (type_ == kNode16)
        {
            /**
             * x86-64 SIMD using SSE2 and optionally AVX-512 instructions
             * See for reference: https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html
             */

            // replicate 8 bit partial key to fill 128 bit register
            const __m128i partial_key_set = _mm_set1_epi8(partial_key);
            // store child key set in 128 bit register
            const __m128i child_key_set = _mm_loadu_epi8(data_);
            // compare partial key set with child key data and store compare bitmask
            // (stores 1 at bit i if keys at position i were equal otherwise 0)
            // using AVX-512 this can be done in one instruction:
            // const __mmask16 cmp_mask = _mm_cmpeq_epi8_mask(partial_key_set, child_key_set);
            const __m128i cmp = _mm_cmpeq_epi8(partial_key_set, child_key_set);
            int cmp_mask = _mm_movemask_epi8(cmp);

            if (cmp_mask)
                // return Node pointer in pointer array at index equal to trailing zeros in cmp_mask
                // Note: Would be cool to have an intrinsic for counting trailing zeros in 16 bit integer
                // but I actually couldn't find one that would have worked. (at least in VC++)
                return *reinterpret_cast<uint64_t*>(&data_[_tzcnt_u32(cmp_mask) * 8 + 16]);
            return NULL;
        }

        if (type_ == kNode48)
        {
            // index 0xFF indicates that the child does not exist
            if (data_[partial_key] != 0xFF)
                return *reinterpret_cast<uint64_t*>(&data_[data_[partial_key] * 8 + 256]);
            return NULL;
        }

        if (type_ == kNode256)
        {
            return *reinterpret_cast<uint64_t*>(&data_[partial_key]);
        }
    }

    void Node::Insert(const uint8_t partial_key, const uint64_t address_value)
    {
        if (IsFull())
            Expand();

        if (type_ == kNode4)
        {
            // TODO: Insert sorted
        }
        else if (type_ == kNode16)
        {
            // TODO: Insert sorted
        }
        else if (type_ == kNode48)
        {
            // TODO: Insert with next free index
        }
        else if (type_ == kNode256)
        {
            data_[partial_key] = address_value;
        }
    }


    void Node::Expand()
    {
        // TODO: Optimize using memory arenas? One for each node type.

        uint8_t* new_data = nullptr;

        if (type_ == kNode4)
        {
            // expand to Node16
            new_data = new uint8_t[144];

            // move data
        }
        else if (type_ == kNode16)
        {
            // expand to Node48
            new_data = new uint8_t[640];

            // move data
        }
        else if (type_ == kNode48)
        {
            // expand to Node256
            new_data = new uint8_t[2048];

            // move data
        }

        delete[] data_;
        data_ = new_data;
    }

    void Node::Destruct()
    {
        // TODO:
    }
}
