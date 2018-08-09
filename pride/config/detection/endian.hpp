
#pragma once

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIA__
    #define PRIDE_LITTLE_ENDIAN
#elif defined(__BYTE_ORDER__)
    #define PRIDE_BIG_ENDIAN
#else
    #define PRIDE_ENDIAN_UNKNOWN
#endif

namespace pride::detection
{
    enum class endian_t
    {
        little,
        big,
        unknown
    };

    constexpr auto get_endian() -> endian_t
    {
    #if defined(PRIDE_LITTLE_ENDIAN)
        return endian_t::little;
    #elif defined(PRIDE_BIG_ENDIAN)
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
} // namespace pride::detection
