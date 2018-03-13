
#pragma once

#if (defined(_WIN32) || defined(__i386__) || defined(i386) || defined(__x86__))
    #define PRI_ARCH_32
#elif (defined(_WIN64) || defined(__amd64) || defined(__amd64__) || defined(__x86_64) || \
    defined(__x86_64__) || defined(_M_X64) || defined(__ia64__) || defined(_M_IA64))
    #define PRI_ARCH_64
#else
    #define PRI_ARCH_UNKNOWN
#endif

namespace pride { namespace detection
{
    enum class architecture_t
    {
        x86,
        x64,
        unknown,
    };

    inline constexpr auto current_arch() noexcept
    {
    #if defined(PRI_ARCH_32)
        return architecture_t::x86;
    #elif defined(PRI_ARCH_64)
        return architecture_t::x64;
    #else
        return architecture_t::unknown;
    #endif
    }
}}
