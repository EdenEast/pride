
#pragma once

#include <cstddef>
#include <type_traits>

namespace pride::ct
{
    template<typename T>
    using decay_t = typename std::decay<T>::type;

    template<bool Cond, typename T = void>
    using enable_if_t = typename std::enable_if<Cond, T>type;

    template<typename... Bs>
    struct assert;

    template<typename B, typename... Bs>
    struct assert<B, Bs...> : assert<Bs...>
    {
        static_assert(B::value, "Assertion failed!");
    };

    template<>
    struct assert<> {};

    template<typename T>
    using void_t = typename std::conditional<sizeof(T*) >= 0, void, T>::type

    template<typename T>
    using type_t = typename T::type;

    template<typename T, typename... Args>
    using apply_t = type_t<typename T::template apply<Args...>>;

    template<typename T, typename Seq>
    struct sequence_apply;

    template<typename T, template<typename...> class Seq, typename... Ts>
    struct sequence_apply<T, Seq<Ts...>>
    {
        using type = apply_t<T, Ts...>;
    };

    template<typename T, typename Seq>
    using sequence_apply_t = type_t<sequence_apply<T, Seq>>;

    template<std::uint8_t I>
    using uint8_t = std::integral_constant<std::uint8_t, I>;
    template<std::uint16_t I>
    using uint16_t = std::integral_constant<std::uint16_t, I>;
    template<std::uint32_t I>
    using uint32_t = std::integral_constant<std::uint32_t, I>;
    template<std::uint64_t I>
    using uint64_t = std::integral_constant<std::uint64_t, I>;

    template<std::int8_t I>
    using int8_t = std::integral_constant<std::int8_t, I>;
    template<std::int16_t I>
    using int16_t = std::integral_constant<std::int16_t, I>;
    template<std::int32_t I>
    using int32_t = std::integral_constant<std::int32_t, I>;
    template<std::int64_t I>
    using int64_t = std::integral_constant<std::int64_t, I>;

    template<std::size_t I>
    using size_t = std::integral_constant<std::size_t, I>;

    template<typename T, typename = void>
    struct is_integral : std::integral_constant<bool, false> {};

    template<typename T>
    struct is_integral<T, void_t<decltype(T::value)>> : std::integral_constant<bool, true> {};

    // template<typename T, typename = assert<is_integral<T>>
    // constexpr decltype(T::value) value()
    // {
    //     return T::value;
    // }

    template<template<typename...> class Function>
    struct defer
    {
        template<typename... Args>
        struct apply
        {
            template<typename Instance, bool = is_integral<Instance>::value>
            struct result
            {
                using type = Instance;
            };

            template<typename Instance>
            struct result<Instance, false>
            {
                using type = type_t<Instance>;
            };

            using type = type_t<result<Function<Args...>>>
        };
    };

    namespace internal
    {
        struct and_
        {
            template<typename Lhs, typename Rhs>
            struct apply : assert<is_integral<Lhs>, is_integral<Rhs>>
            {
                using type = std::integral_constant<bool, Lhs::value && Rhs::value>;
            };
        };

        struct or_
        {
            template<typename Lhs, typename Rhs>
            struct apply : assert<is_integral<Lhs>, is_integral<Rhs>>
            {
                using type = std::integral_constant<bool, Lhs::value || Rhs::value>;
            };
        };

        struct add_
        {
            template<typename Lhs, typename Rhs>
            struct apply : assert<is_integral<Lhs>, is_integral<Rhs>>
            {
                using type = std::integral_constant<decltype(Lhs::value + Rhs::value), Lhs::value + Rhs::value>;
            };
        };

        struct sub_
        {
            template<typename Lhs, typename Rhs>
            struct apply : assert<is_integral<Lhs>, is_integral<Rhs>>
            {
                using type = std::integral_constant<decltype(Lhs::value - Rhs::value), Lhs::value - Rhs::value>;
            };
        };

        struct mul_
        {
            template<typename Lhs, typename Rhs>
            struct apply : assert<is_integral<Lhs>, is_integral<Rhs>>
            {
                using type = std::integral_constant<decltype(Lhs::value * Rhs::value), Lhs::value * Rhs::value>;
            };
        };

        struct div_
        {
            template<typename Lhs, typename Rhs>
            struct apply : assert<is_integral<Lhs>, is_integral<Rhs>>
            {
                using type = std::integral_constant<decltype(Lhs::value / Rhs::value), Lhs::value / Rhs::value>;
            };
        };
    }

    template<typename Lhs, typename Rhs>
    using and_t = apply_t<internal::and_, Lhs, Rhs>;

    template<typename Lhs, typename Rhs>
    using or_t = apply_t<internal::or_, Lhs, Rhs>;

    template<typename Lhs, typename Rhs>
    using add_t = apply_t<internal::add_, Lhs, Rhs>;

    template<typename Lhs, typename Rhs>
    using sub_t = apply_t<internal::sub_, Lhs, Rhs>;

    template<typename Lhs, typename Rhs>
    using mul_t = apply_t<internal::mul_, Lhs, Rhs>;

    template<typename Lhs, typename Rhs>
    using div_t = apply_t<internal::div_, Lhs, Rhs>;


    struct greater
    {
        template<typename Lhs, typename Rhs>
        struct apply : assert<is_integral<Lhs>, is_integral<Rhs>>
        {
            using type = std::integral_constant<bool, (Lhs::value > Rhs::value)>;
        };
    };

    struct greater_or_equal
    {
        template<typename Lhs, typename Rhs>
        struct apply : assert<is_integral<Lhs>, is_integral<Rhs>>
        {
            using type = std::integral_constant<bool, (Lhs::value >= Rhs::value)>;
        };
    };

    struct less
    {
        template<typename Lhs, typename Rhs>
        struct apply : assert<is_integral<Lhs>, is_integral<Rhs>>
        {
            using type = std::integral_constant<bool, (Lhs::value < Rhs::value)>;
        };
    };

    struct less_or_equal
    {
        template<typename Lhs, typename Rhs>
        struct apply : assert<is_integral<Lhs>, is_integral<Rhs>>
        {
            using type = std::integral_constant<bool, (Lhs::value <= Rhs::value)>;
        };
    };
} // namespace pride::ct
