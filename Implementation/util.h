/**
 * __unreachable()
 *
 * Unreachable.
 */
#ifdef __GNUC__ // GCC 4.8+, Clang, Intel and other compilers compatible with GCC (-std=c++0x or above)

[[noreturn]] inline __attribute__((always_inline)) void __unreachable()
{ __builtin_unreachable(); }

#elif defined(_MSC_VER) // MSVC

[[noreturn]] __forceinline void __unreachable()
{
    __assume(false);
}

#endif
