#pragma once

#include <immintrin.h>

/**
 * _mm_cmplt_epu8(__m128i a, __m128i b)
 *
 * Compare packed unsigned 8-bit integers in a and b for less-than and return result.
 */
#ifdef __GNUC__ // GCC 4.8+, Clang, Intel and other compilers compatible with GCC (-std=c++0x or above)
inline __attribute__((always_inline)) __m128i _mm_cmplt_epu8(__m128i a, __m128i b) { return _mm_xor_si128(_mm_cmpeq_epi8(_mm_max_epu8(b, a), b), _mm_set1_epi8(-1)); }
#elif defined(_MSC_VER) // MSVC

__forceinline __m128i _mm_cmplt_epu8(const __m128i a, const __m128i b)
{ return _mm_xor_si128(_mm_cmpeq_epi8(_mm_max_epu8(a, b), a), _mm_set1_epi8(-1)); }

#endif

/**
 * __ctz(uint16_t val)
 *
 * Counts trailing zeros.
 */
#ifdef __GNUC__ // GCC 4.8+, Clang, Intel and other compilers compatible with GCC (-std=c++0x or above)
inline __attribute__((always_inline)) unsigned __ctz(uint16_t val) { return __builtin_ctz(val); }
#elif defined(_MSC_VER) // MSVC

__forceinline unsigned __ctz(const uint16_t val)
{ return _tzcnt_u32(val); }

#endif