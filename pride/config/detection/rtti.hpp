
#pragma once

#if defined(__cpp_rtti) || defined(__GXX_RTTI) || defined(_CPPRTTI)
    #define PRI_RTTI_ENABLED
#else
    #define PRI_RTTI_DISABLED
#endif

namespace pride { namespace detection
{
    inline constexpr auto rtti() noexcept
    {
    #if defined(PRI_RTII_ENABLED)
        return true;
    #else
        return false;
    #endif
    }
}}
