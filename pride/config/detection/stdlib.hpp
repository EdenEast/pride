
#pragma once

#if defined(_LIBCPP_VERSION)
    #define PRI_STDLIB_LIBCXX
#elif defined(__GLIBCXX__)
    #define PRI_STDLIB_STDLIBCXX
#else
    #define PRI_STDLIB_UNKNOWN
#endif

namespace pride { namespace detection
{
    enum class stdlib_t
    {
        libcxx,
        libstdcxx,
        unknown
    };

    constexpr auto current_stdlib() noexcept
    {
    #if defined(PRI_STDLIB_LIBCXX)
        return stdlib_t::libcxx;
    #elif defined(PRI_STDLIB_STDLIBCXX)
        return stdlib_t::libstdcxx;
    #else
        return stdlib_t::unknown;
    #endif
    }
}}
