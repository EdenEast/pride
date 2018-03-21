
#pragma once

#include "../config.hpp"
namespace pride
{
#if defined(PRI_COMPILER_CLANG) || defined(PRI_COMPILER_GCC)
    inline constexpr auto likely(bool x) noexcept -> bool
    {
        return __builtin_expect(x, true);
    }

    inline constexpr auto unlikely(bool x) noexcept -> bool
    {
        return __builtin_expect(x, false);
    }
#else
    inline constexpr auto likely(bool x) noexcept -> bool
    {
        return x;
    }

    inline constexpr auto unlikely(bool x) noexcept -> bool
    {
        return x;
    }
#endif
}
