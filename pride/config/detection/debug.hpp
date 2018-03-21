
#pragma once

#if !defined(NDEBUG)
    #define PRI_DEBUG
#endif

namespace pride { namespace detection
{
    inline constexpr auto debug() noexcept -> bool
    {
    #if defined(PRI_DEBUG)
        return true;
    #else
        return false;
    #endif
    }
}}
