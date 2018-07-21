
#pragma once

#include "../../config/detection/os.hpp"
#include "../../config/include/windows.hpp"
#include <chrono>
#include <ctime>
#include <string>
#include <thread>

namespace pride::log::detail
{
    inline std::chrono::system_clock::time_point now()
    {
        return std::chrono::system_clock::now();
    }

    inline std::tm local_time(const std::time_t& tt)
    {
        std::tm tm;
#if defined(PRI_OS_WINDOWS)
        localtime_s(&tm, &tt);
#else
        localtime_r(&tt, &tm);
#endif
        return tm;
    }

    inline std::tm local_time()
    {
        std::time_t n = time(nullptr);
        return local_time(n);
    }

    inline std::tm gm_time(const std::time_t& tt)
    {
        std::tm tm;
#if defined(PRI_OS_WINDOWS)
        gmtime_s(&tm, &tt);
#else
        gmtime_r(&tt, &tm);
#endif
        return tm;
    }

    inline std::tm gm_time()
    {
        std::time_t n = time(nullptr);
        return gm_time(n);
    }

    inline bool operator==(const std::tm& tm1, const std::tm& tm2)
    {
        return (tm1.tm_sec == tm2.tm_sec && tm1.tm_min == tm2.tm_min && tm1.tm_hour == tm2.tm_hour && tm1.tm_mday == tm2.tm_mday &&
            tm1.tm_mon == tm2.tm_mon && tm1.tm_year == tm2.tm_year && tm1.tm_isdst == tm2.tm_isdst);
    }

    inline bool operator!=(const std::tm &tm1, const std::tm &tm2)
    {
        return !(tm1 == tm2);
    }
}
