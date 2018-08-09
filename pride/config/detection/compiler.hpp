
#pragma once

#if defined(__clang__)
    #define PRIDE_COMPILER_CLANG
#elif defined(__GNUC__)
    #define PRIDE_COMPILER_GCC
#elif defined(_MSC_VER)
    #define PRIDE_COMPILER_MSVC
#else
    #define PRIDE_COMPILER_UNKNOWN
#endif

namespace pride::detection
{
    enum class compiler_t
    {
        clang,
        gcc,
        msvc,
        unknown
    };

    inline constexpr auto current_compiler() noexcept -> compiler_t
    {
    #if defined(PRIDE_COMPILER_CLANG)
        return compiler_t::clang;
    #elif defined(PRIDE_COMPILER_GCC)
        return compiler_t::gcc;
    #elif defined(PRIDE_COMPILER_MSVC)
        return compiler_t::msvc;
    #else
        return compiler_t::unknown;
    #endif
    }
} // namespace pride::detection
