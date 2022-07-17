#pragma once

#include <immintrin.h>

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