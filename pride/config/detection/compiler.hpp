
#pragma once

#if defined(__clang__)
    #define PRI_COMPILER_CLANG
#elif defined(__GNUC__)
    #define PRI_COMPILER_GCC
#elif defined(_MSC_VER)
    #define PRI_COMPILER_MSVC
#else
    #define PRI_COMPILER_UNKNOWN
#endif

namespace pride { namespace detection
{
    enum class compiler_t
    {
        clang,
        gcc,
        msvc,
        unknown
    };

    inline constexpr auto current_compiler() noexcept
    {
    #if defined(PRI_COMPILER_CLANG)
        return compiler_t::clang;
    #elif defined(PRI_COMPILER_GCC)
        return compiler_t::gcc;
    #elif defined(PRI_COMPILER_MSVC)
        return compiler_t::msvc;
    #else
        return compiler_t::unknown;
    #endif
    }
}}
