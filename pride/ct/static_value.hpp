
#pragma once

namespace pride { namespace ct
{
    template<typename T, T Value>
    struct static_value
    {
        using value_type = T;

        constexpr static_value() = default;
        static constexpr value_type value = Value;

        constexpr operator value_type() const { return Value; }
        constexpr value_type get() const { return Value; }

        friend constexpr bool operator==(const static_value& lhs, const value_type rhs)
        {
            return lhs.get() == rhs;
        }

        friend constexpr bool operator==(const value_type lhs, const static_value& rhs)
        {
            return rhs == lhs;
        }

        friend constexpr bool operator!=(const static_value& lhs, const value_type rhs)
        {
            return !(lhs == rhs);
        }

        friend constexpr bool operator!=(const value_type lhs, const static_value& rhs)
        {
            return !(lhs == rhs);
        }
    };

    template<typename T, T Value>
    constexpr T static_value<T, Value>::value;
}}

#define PRI_STATIC_VALUE(x) ::pride::ct::static_value<decltype(x), x>
