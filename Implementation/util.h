#pragma once

#include <cmath>

inline bool IsBigEndian()
{
    union
    {
        uint32_t i;
        char c[4];
    } eint = {0x01020304};

    return eint.c[0] == 1;
}

inline uint32_t SwapEndianess(const uint32_t value)
{
    return (value & 0xFF000000) >> 24
        | (value & 0xFF0000) >> 8
        | (value & 0xFF00) << 8
        | ((unsigned)value & 0xFF) << 24;
}

/**
 * __unreachable()
 *
 * Unreachable.
 */
#ifdef __GNUC__ // GCC 4.8+, Clang, Intel and other compilers compatible with GCC (-std=c++0x or above)
[[noreturn]] inline __attribute__((always_inline)) void __unreachable() { __builtin_unreachable(); }
#elif defined(_MSC_VER) // MSVC

[[noreturn]] __forceinline void __unreachable()
{
    __assume(false);
}

#endif
