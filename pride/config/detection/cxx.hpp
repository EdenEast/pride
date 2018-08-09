
#pragma once

#if __cplusplus >= 201703L
    #define PRIDE_CXX_17
#elif __cplusplus >= 201402L
    #define PRIDE_CXX_14
#elif __cplusplus >= 201103L
    #define PRIDE_CXX_11
#elif __cplusplus >= 199711L
    #define PRIDE_CXX_98
#else
    #define PRIDE_CXX_UNKNOWN
#endif

namespace pride::detection
{
    enum class cxx_version_t
    {
        unknown,
        cxx98,
        cxx11,
        cxx14,
        cxx17
    };

    inline auto get_cxx_version() -> cxx_version_t
    {
    #if defined(PRIDE_CXX_17)
        return cxx_version_t::cxx17;
    #elif defined(PRIDE_CXX_14)
        return cxx_version_t::cxx14;
    #elif defined(PRIDE_CXX_11)
        return cxx_version_t::cxx11;
    #elif defined(PRIDE_CXX_98)
        return cxx_version_t::cxx98;
    #else
        return cxx_version_t::unknown;
    #endif
    }

    inline bool is_cxx_ge(cxx_version_t version)
    {
        return get_cxx_version() >= version;
    }
} // namespace pride::detection
