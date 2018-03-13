
#pragma once

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIA__
    #define PRI_LITTLE_ENDIAN
#elif defined(__BYTE_ORDER__)
    #define PRI_BIG_ENDIAN
#else
    #define PRI_ENDIAN_UNKNOWN
#endif

namespace pride { namespace detection
{
    enum class endian_t
    {
        little,
        big,
        unknown
    };

    constexpr endian_t get_endian()
    {
    #if defined(PRI_LITTLE_ENDIAN)
        return endian_t::little;
    #elif defined(PRI_BIG_ENDIAN)
        return endian_t::big;
    #else
        return endian_t::unknown;
    #endif
    }

    constexpr bool is_little_endian()
    {
        return get_endian() == endian_t::little;
    }

    constexpr bool is_big_endian()
    {
        return get_endian() == endian_t::big;
    }
}}
