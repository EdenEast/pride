
#pragma once

#if !defined(NDEBUG)
    #define PRIDE_DEBUG
#endif

namespace pride { namespace detection
{
    inline constexpr auto debug() noexcept -> bool
    {
    #if defined(PRIDE_DEBUG)
        return true;
    #else
        return false;
    #endif
    }
}}
