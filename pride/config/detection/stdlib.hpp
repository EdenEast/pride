
#pragma once

#if defined(_LIBCPP_VERSION)
    #define PRIDE_STDLIB_LIBCXX
#elif defined(__GLIBCXX__)
    #define PRIDE_STDLIB_STDLIBCXX
#else
    #define PRIDE_STDLIB_UNKNOWN
#endif

namespace pride::detection
{
    enum class stdlib_t
    {
        libcxx,
        libstdcxx,
        unknown
    };

    constexpr auto current_stdlib() noexcept -> stdlib_t
    {
    #if defined(PRIDE_STDLIB_LIBCXX)
        return stdlib_t::libcxx;
    #elif defined(PRIDE_STDLIB_STDLIBCXX)
        return stdlib_t::libstdcxx;
    #else
        return stdlib_t::unknown;
    #endif
    }
} // namespace pride::detection
