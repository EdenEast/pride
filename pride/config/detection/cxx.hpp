
#pragma once

#if __cplusplus >= 201703L
    #define PRI_CXX_17
#elif __cplusplus >= 201402L
    #define PRI_CXX_14
#elif __cplusplus >= 201103L
    #define PRI_CXX_11
#elif __cplusplus >= 199711L
    #define PRI_CXX_98
#else
    #define PRI_CXX_UNKNOWN
#endif

namespace pride { namespace detection
{
    enum class cxx_version_t
    {
        unknown,
        cxx98,
        cxx11,
        cxx14,
        cxx17
    };

    cxx_version_t get_cxx_version()
    {
    #if defined(PRI_CXX_17)
        return cxx_version_t::cxx17;
    #elif defined(PRI_CXX_14)
        return cxx_version_t::cxx14;
    #elif defined(PRI_CXX_11)
        return cxx_version_t::cxx11;
    #elif defined(PRI_CXX_98)
        return cxx_version_t::cxx98;
    #else
        return cxx_version_t::unknown;
    #endif
    }

    bool is_cxx_ge(cxx_version_t version)
    {
        return get_cxx_version() >= version;
    }
}}
