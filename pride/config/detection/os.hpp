
#pragma once

#if defined(__unix__) || defined(__unix)
    #if defined(__ANDROID__)
        #define PRI_OS_ANDROID
    #elif defined(__linux__)
        #define PRI_OS_LINUX
    #else
        #define PRI_OS_UNKNOWN
    #endif
#elif defined(_WIN64) || defined(_WIN32) || defined(__CYGWIN32__) || defined(__MINGW32__)
    #define PRI_OS_WINDOWS
#elif defined(MACOSX) || defined(__DARWIN__) || defined(__APPLE__)
    #define PRI_OS_MACOSX
#else
    #define PRI_OS_UNKNOWN
#endif

namespace pride { namespace detection
{
    enum class operating_system_t
    {
        linux_unix,
        windows,
        macosx,
        android,
        unknown
    };

    inline constexpr operating_system_t current_operating_system() noexcept
    {
    #if defined(PRI_OS_WINDOWS)
        return operating_system_t::windows;
    #elif defined(PRI_OS_MACOSX)
        return operating_system_t::macosx;
    #elif defined(PRI_OS_LINUX)
        return operating_system_t::linux_unix;
    #elif defined(PRI_OS_ANDROID)
        return operating_system_t::android;
    #else
        return operating_system_t::unknown;
    #endif
    }

    inline constexpr auto on_unix() noexcept -> bool
    {
        return current_operating_system() == operating_system_t::android ||
               current_operating_system() == operating_system_t::linux_unix;
    }

    inline constexpr auto on_windows() noexcept -> bool
    {
        return current_operating_system() == operating_system_t::windows;
    }

    inline constexpr auto on_macosx() noexcept -> bool
    {
        return current_operating_system() == operating_system_t::macosx;
    }
}}

