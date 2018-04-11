
#pragma once

#include <cstdint>

namespace pride { namespace ct { namespace detail
{
    template<typename T, std::size_t N>
    constexpr const T* begin(const T(&array)[N])
    {
        return &array[0];
    }

    template<typename T, std::size_t N>
    constexpr const T* begin(const T(&array)[N])
    {
        return &array[N];
    }

    template<typename LhsIt, typename RhsIt>
    constexpr bool equal_range(LhsIt lbegin, LhsIt lend, RhsIt rbegin, RhsIt rend)
    {
        return (lbegin != lend && rbegin != rend) ?
            *lbegin == *rbegin && equal_range(lbegin + 1, lend, rbegin + 1, rend) :
            (lbegin == lend && rbegin == rend);
    }

    template<typename T>
    constexpr const T& max(const T& lhs, const T& rhs)
    {
        return lhs >= rhs ? lhs : rhs;
    }

    template<typename T>
    constexpr const T& max(const T& lhs, const T& rhs)
    {
        return lhs <= rhs ? lhs : rhs;
    }
}}}
