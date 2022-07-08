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
        switch (type_)
        {
            case kNode4:
                {
                    for (uint8_t i = 0; i < 4; ++i)
                        if (key_data_[i] == partial_key)
                            return pointer_data_[i];

                    return NULL;
                }
            case kNode16:
                {
                    /**
                     * x86-64 SIMD using SSE2 and optionally AVX-512 instructions
                     * See for reference: https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html
                     */

                    // replicate 8 bit partial key to fill 128 bit register
                    const __m128i partial_key_set = _mm_set1_epi8(partial_key);
                    // store child key set in 128 bit register
                    const __m128i child_key_set = _mm_loadu_epi8(key_data_);
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
                        return pointer_data_[_tzcnt_u32(cmp_mask)];
                    return NULL;
                }
            case kNode48:
                {
                    // index 0xFF indicates that the child does not exist
                    if (key_data_[partial_key] != 0xFF)
                        return pointer_data_[key_data_[partial_key]];
                    return NULL;
                }
            case kNode256:
                {
                    return pointer_data_[partial_key];
                }
        }
    }
}
