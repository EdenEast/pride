
#pragma once

#if defined(__cpp_rtti) || defined(__GXX_RTTI) || defined(_CPPRTTI)
    #define PRIDE_RTTI_ENABLED
#else
    #define PRIDE_RTTI_DISABLED
#endif

namespace pride { namespace detection
{
    inline constexpr auto rtti() noexcept -> bool
    {
    #if defined(PRIDE_RTII_ENABLED)
        return true;
    #else
        return false;
    #endif
    }
}}
