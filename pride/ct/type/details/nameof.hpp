
#pragma once

#include "../../static_value.hpp"
#include "../type_tag.hpp"
#include "pretty_function.hpp"
#include <type_traits>
#include <cstdint>

namespace pride { namespace ct
{
    template<typename T>
    constexpr cstring_t nameof();

    template<typename T, T Value>
    constexpr cstring_t nameof();

    namespace detail
    {
        template<typename T>
        struct indirection {};

        template<typename T>
        using is_void = typename std::conditional<sizeof(T*) >= 0, void, T>::type;

        template<typename T, typename = void>
        struct typename_length : std::integral_constant<
            std::size_t,
            pride::ct::nameof<T>().length()
        > {};

        template<typename T, typename Class>
        struct typename_length<T Class::*, void> : std::integral_constant<
            std::size_t,
            pride::ct::nameof<T Class::*>().length()
        > {};

        template<typename Enum>
        struct typename_length<Enum, typename std::enable_if<std::is_enum<Enum>::value>::type> :
            std::integral_constant<
                std::size_t,
                pride::ct::nameof<Enum>().length()
            >
        {};

        template<typename T, typename = void>
        struct nameof_impl
        {
            static constexpr cstring_t apply()
            {
                return pride::ct::pretty_function::type<T>().pad(
                    PRI_TYPE_PRETTY_FUNCTION_LEFT,
                    PRI_TYPE_PRETTY_FUNCTION_RIGHT
                );
            }
        };

        template<typename T>
        struct nameof_impl<T, is_void<decltype(T::nameof())>>
        {
            static constexpr cstring_t apply()
            {
                return T::nameof();
            }
        };

        template<typename T, T Value>
        struct nameof_impl<indirection<pride::ct::static_value<T, Value>>, void>
        {
            static constexpr cstring_t apply()
            {
                 return pretty_function::value<T, Value>().pad(
                     PRI_VALUE_PRETTY_FUNCTION_LEFT + pride::ct::detail::typename_length<T>::value + PRI_VALUE_PRETTY_FUNCTION_SEPARATION,
                     PRI_VALUE_PRETTY_FUNCTION_RIGHT
                 );
            }
        };
    }

    template<typename T>
    constexpr cstring_t nameof(type_tag<T>)
    {
        return detail::nameof_impl<T>::apply();
    }

    template<typename T, T Value>
    constexpr cstring_t nameof(pride::ct::static_value<T, Value>)
    {
        return detail::nameof_impl<detail::indirection<pride::ct::static_value<T, Value>>>::apply();
    }

    template<typename T>
    constexpr cstring_t nameof()
    {
        return nameof(type_tag<T>());
    }

    template<typename T, T Value>
    constexpr cstring_t nameof()
    {
        return nameof(static_value<T, Value>());
    }
}}
