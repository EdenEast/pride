// Taken from https://github.com/tim42/tools/blob/master/ct_list.hpp
#pragma once

#include <memory>
#include <type_traits>
#include <tuple>

namespace pride { namespace ct
{
    namespace list
    {
        //
        // ─── GET TYPE INDEX AND HAS VALUE ───────────────────────────────────────────────
        //
        namespace internal
        {
            // return the type at a given index
            template<size_t Index, typename... Types>
            struct type_at_index {};
            template<typename Current, typename... Other, size_t Index>
            struct type_at_index<Index, Current, Other...> : public type_at_index<Index - 1, Other...> {};
            template<typename Current, typename... Other>
            struct type_at_index<0, Current, Other...> { using type = Current; };
            template<size_t Index>
            struct type_at_index<Index> { static_assert(!(Index + 1), "Out of range access in get_type"); };

            template<typename List, size_t Index>
            struct type_at_index_list {};
            template<template<typename...> class List, typename... Types, size_t Index>
            struct type_at_index_list<List<Types...>, Index> : public type_at_index<Index, Types...> {};

            // return the index of a given type
            template<size_t Index, typename Type, typename... Types>
            struct type_index { static constexpr long index = -1; };
            template<size_t Index, typename Current, typename... Types>
            struct type_index<Index, Current, Current, Types...> { static constexpr long index = Index; };
            template<size_t Index, typename Type, typename Current, typename... Types>
            struct type_index<Index, Type, Current, Types...> : public type_index < Index + 1, Type, Types... > {};
            template<size_t Index, typename Type>
            struct type_index<Index, Type> { static constexpr long index = -1; };

            template<typename List, typename Type>
            struct type_index_list {};
            template<template<typename...> class List, typename... Types, typename Type>
            struct type_index_list<List<Types...>, Type> : public type_index<0, Type, Types...> {};

            template<typename List, typename Type>
            struct has_type : public std::conditional<(internal::type_index_list<List, Type>::index != -1), std::true_type, std::false_type> {};
        } // namespace internal

        // return the type at a given index. static_assert will be tripped if out of range
        template<typename List, size_t Index>
        using get_type = typename internal::type_at_index_list<List, Index>::type;

        // return the index of a given type in the list. -1 if the value is not found
        template<typename List, typename Type>
        static constexpr long index_of = internal::type_index_list<List, Type>::index;

        // return bool to check if type is in list
        template<typename List, typename Type>
        static constexpr bool has_type = (internal::type_index_list<List, Type>::index != -1);

        //
        // ─── APPEND AND PREPEND ─────────────────────────────────────────────────────────
        //
        namespace internal
        {
            template<typename List, typename... Y>
            struct prepend {};
            template<typename List, typename... Y>
            struct append {};

            template<template<typename...> class Type, typename... Others, typename... Types>
            struct prepend<Type<Others...>, Types...> { using type = Type<Types..., Others...>; };
            template<template<typename...> class Type, typename... Others, typename... Types>
            struct append<Type<Others...>, Types...> { using type = Type<Others..., Types...>; };
        }

        // prepend variadic types to the list
        template<typename List, typename... T>
        using prepend = typename internal::prepend<List, T...>::type;

        // append variadic types to the list
        template<typename List, typename... T>
        using append = typename internal::append<List, T...>::type;

        template<bool Cond, typename List, typename... Y>
        using append_if = typename std::conditional<Cond, append<List, Y...>, List>::type;

        template<bool Cond, typename List, typename... Y>
        using prepend_if = typename std::conditional<Cond, prepend<List, Y...>, List>::type;

        //
        // ─── MERGE AND EXTRACT ──────────────────────────────────────────────────────────
        //
        template<typename List1, typename List2>
        struct merge
        {
            using as_first = void;
            using as_second = void;
        };

        template<template<typename...> class List1, typename... Types1, template<typename...> class List2, typename... Types2>
        struct merge<List1<Types1...>, List2<Types2...>>
        {
            template<template<typename...> class ResultT>
            using as = ResultT<Types1..., Types2...>;

            using as_first = List1<Types1..., Types2...>;
            using as_second = List2<Types1..., Types2...>;
        };

        // extract template arguments and forward them to another base type
        template<typename List>
        struct extract {};

        template<template<typename...> class List, typename... Types>
        struct extract<List<Types...>>
        {
            template<template<typename...> class Result>
            using as = Result<Types...>;
        };

        //
        // ─── SIZE AND IS_SAME ───────────────────────────────────────────────────────────
        //
        namespace internal
        {
            template<typename List>
            struct size {};
            template<template<typename...> class List, typename... Types>
            struct size<List<Types...>> { static constexpr size_t count = sizeof...(Types); };

            template<typename List1, typename List2>
            struct is_same_base : public std::false_type {};
            template<template<typename...> class L1, typename... Types1, typename... Types2>
            struct is_same_base<L1<Types1...>, L1<Types2...>> : public std::true_type {};
        }

        // the number of template arguments in the list
        template<typename List>
        static constexpr size_t size = internal::size<List>::count;

        // return true if both list are the same base type
        template<typename List1, typename List2>
        static constexpr bool is_same_base = internal::is_same_base<List1, List2>::value;

        //
        // ─── REMOVE ─────────────────────────────────────────────────────────────────────
        //
        namespace internal
        {
            template<typename List, long Index, typename... Types>
            struct remove { using type = List; };
            template<typename List, long Index, typename Current, typename... Types>
            struct remove<List, Index, Current, Types...> :
            public remove<append_if<Index != 0, List, Current>, Index - 1, Types...> {};

            template<typename List, template<typename X> typename Pred, bool ExpectedResult, typename... Types>
            struct remove_if { using type = List; };
            template<typename List, template<typename X> typename Pred, bool ExpectedResult, typename Current, typename... Types>
            struct remove_if<List, Pred, ExpectedResult, Current, Types...> :
            public remove_if<append_if<Pred<Current>::value != ExpectedResult, List, Current>, Pred, ExpectedResult, Types...> {};

            template<typename List, size_t Index>
            struct remove_entry {};
            template<template<typename...> class List, typename... Types, size_t Index>
            struct remove_entry<List<Types...>, Index> : public remove<List<>, Index, Types...> {};

            template<typename List, template<typename X> typename Pred, bool ExpectedResult>
            struct remove_if_entry {};
            template<template<typename...> class List, typename... Types, template<typename X> typename Pred, bool ExpectedResult>
            struct remove_if_entry<List<Types...>, Pred, ExpectedResult> : public remove_if<List<>, Pred, ExpectedResult, Types...> {};

            template<typename X>
            struct remove_type
            {
                template<typename Y>
                struct pred : public std::is_same<X, Y> {};

                template<typename List>
                using list_op = remove_if_entry<List, pred, true>;
            };
        }

        // remove a type at a given index
        template<typename List, size_t Index>
        using remove = typename internal::remove_entry<List, Index>::type;

        // remove a given type
        template<typename List, typename ToRemove>
        using remove_type = typename internal::remove_type<ToRemove>::template list_op<List>::type;

        // remove elements if condition passes
        template<typename List, template<typename Elem> typename Pred>
        using remove_if = typename internal::remove_if_entry<List, Pred, true>::type;

        // return a list containing only elements where the filter is true
        template<typename List, template<typename Elem> typename Pred>
        using filter = typename internal::remove_if_entry<List, Pred, false>::type;

        //
        // ─── MAKE UNIQUE ─────────────────────────────────────────────────
        //
        namespace internal
        {
            template<typename List, typename... Types>
            struct make_unique { using type = List; };
            template<typename List, typename Current, typename... Types>
            struct make_unique<List, Current, Types...> : public make_unique<append_if<!list::has_type<List, Current>, List, Current>, Types...> {};
            template<typename List>
            struct make_unique_list {};
            template<template<typename...> class List, typename... Types>
            struct make_unique_list<List<Types...>> : public make_unique<List<>, Types...> {};
        }

        template<typename List>
        using make_unique = typename internal::make_unique_list<List>::type;

        //
        // ─── FOREACH ────────────────────────────────────────────────────────────────────
        //
        namespace internal
        {
            template<typename List, template<typename X> typename Pred>
            struct for_each {};
            template<template<typename...> class List, typename... Types, template<typename X> typename Pred>
            struct for_each<List<Types...>, Pred> { using type = List<Pred<Types>...>; };
        }

        // apply predicate to every type in the list
        template<typename List, template<typename Elem> typename Pred>
        using for_each = typename internal::for_each<List, Pred>::type;

        //
        // ─── REVERSE ────────────────────────────────────────────────────────────────────
        //
        namespace internal
        {
            template<typename List, typename... Types>
            struct reverse { using type = List; };
            template<typename List, typename Current, typename... Others>
            struct reverse<List, Current, Others...> : public reverse<list::prepend<List, Current>, Others...> {};

            template<typename List>
            struct reverse_list {};
            template<template<typename...> class List, typename... Types>
            struct reverse_list<List<Types...>> : public reverse<List<>, Types...>{};
        }

        template<typename List>
        using reverse = typename internal::reverse_list<List>::type;
    }

    template<typename... Types>
    class type_list;
}}

#if defined(PRI_USE_CT_TEST)
namespace pride::ct
{
    static_assert(std::is_same_v<list::get_type<type_list<int, double, float>, 2>, float>, "list::get_type: failed");
    static_assert(std::is_same_v<list::get_type<type_list<int, double, float>, 0>, int>, "list::get_type: failed");

    static_assert(list::index_of<type_list<int, double, float>, int> == 0, "list::index_of: failed");
    static_assert(list::index_of<type_list<int, double, float>, float> == 2, "list::index_of: failed");
    static_assert(list::index_of<type_list<int, double, float>, char> == -1, "list::index_of: failed");

    static_assert(list::has_type<type_list<int, double, float>, char> == false, "list::has_type: failed");
    static_assert(list::has_type<type_list<int, double, float>, double> == true, "list::has_type: failed");
    static_assert(list::has_type<type_list<int, double, float>, int> == true, "list::has_type: failed");

    static_assert(std::is_same_v<list::append<type_list<int, double, float>, char, int>, type_list<int, double, float, char, int>>, "list::append: failed");
    static_assert(std::is_same_v<list::append<type_list<int, double, float>, int>, type_list<int, double, float, int>>, "list::append: failed");
    static_assert(std::is_same_v<list::append<type_list<int, double, float>>, type_list<int, double, float>>, "list::append: failed");

    static_assert(std::is_same_v<list::prepend<type_list<int, double, float>, char, int>, type_list<char, int, int, double, float>>, "list::prepend: failed");
    static_assert(std::is_same_v<list::prepend<type_list<int, double, float>, int>, type_list<int, int, double, float>>, "list::prepend: failed");
    static_assert(std::is_same_v<list::prepend<type_list<int, double, float>>, type_list<int, double, float>>, "list::prepend: failed");

    static_assert(std::is_same_v<list::append_if<true, type_list<int, double, float>, char, int>, type_list<int, double, float, char, int>>, "list::append_if: failed");
    static_assert(std::is_same_v<list::append_if<true, type_list<int, double, float>, int>, type_list<int, double, float, int>>, "list::append_if: failed");
    static_assert(std::is_same_v<list::append_if<true, type_list<int, double, float>>, type_list<int, double, float>>, "list::append_if: failed");

    static_assert(std::is_same_v<list::prepend_if<true, type_list<int, double, float>, char, int>, type_list<char, int, int, double, float>>, "list::prepend_if: failed");
    static_assert(std::is_same_v<list::prepend_if<true, type_list<int, double, float>, int>, type_list<int, int, double, float>>, "list::prepend_if: failed");
    static_assert(std::is_same_v<list::prepend_if<true, type_list<int, double, float>>, type_list<int, double, float>>, "list::prepend_if: failed");

    static_assert(std::is_same_v<list::append_if<false, type_list<int, double, float>, char, int>, type_list<int, double, float>>, "list::append_if: failed");
    static_assert(std::is_same_v<list::append_if<false, type_list<int, double, float>, int>, type_list<int, double, float>>, "list::append_if: failed");
    static_assert(std::is_same_v<list::append_if<false, type_list<int, double, float>>, type_list<int, double, float>>, "list::append_if: failed");

    static_assert(std::is_same_v<list::prepend_if<false, type_list<int, double, float>, char, int>, type_list<int, double, float>>, "list::prepend_if: failed");
    static_assert(std::is_same_v<list::prepend_if<false, type_list<int, double, float>, int>, type_list<int, double, float>>, "list::prepend_if: failed");
    static_assert(std::is_same_v<list::prepend_if<false, type_list<int, double, float>>, type_list<int, double, float>>, "list::prepend_if: failed");

    static_assert(std::is_same_v<typename list::merge<type_list<>, type_list<>>::as_first, type_list<>>, "list::merge: failed");
    static_assert(std::is_same_v<typename list::merge<type_list<>, type_list<int, double, float>>::as_first, type_list<int, double, float>>, "list::merge: failed");
    static_assert(std::is_same_v<typename list::merge<type_list<int, double>, type_list<int, double, float>>::as_first, type_list<int, double, int, double, float>>, "list::merge: failed");

    static_assert(std::is_same_v<typename list::extract<type_list<int, double, float>>::as<type_list>, type_list<int, double, float>>, "list::extract: failed");
    static_assert(std::is_same_v<typename list::extract<type_list<int, double, float>>::as<std::tuple>, std::tuple<int, double, float>>, "list::extract: failed");
    static_assert(std::is_same_v<typename list::extract<type_list<>>::as<type_list>, type_list<>>, "list::extract: failed");
    static_assert(std::is_same_v<typename list::extract<type_list<>>::as<std::tuple>, std::tuple<>>, "list::extract: failed");

    static_assert(list::size<type_list<>> == 0, "list::size: failed");
    static_assert(list::size<type_list<int>> == 1, "list::size: failed");
    static_assert(list::size<type_list<int, int, int, int, type_list<int, int>>> == 5, "list::size: failed");

    static_assert(list::is_same_base<type_list<int, int, int, int>, type_list<char, double>> == true, "list::same_base: failed");
    static_assert(list::is_same_base<type_list<>, type_list<char, double>> == true, "list::same_base: failed");
    static_assert(list::is_same_base<type_list<>, std::tuple<char, double>> == false, "list::same_base: failed");
    static_assert(list::is_same_base<std::tuple<>, type_list<char, double>> == false, "list::same_base: failed");

    static_assert(std::is_same_v<list::remove<type_list<int, long, double, char>, 0>, type_list<long, double, char>>, "list::remove: failed");
    static_assert(std::is_same_v<list::remove<type_list<int, long, double, char>, 1>, type_list<int, double, char>>, "list::remove: failed");
    static_assert(std::is_same_v<list::remove<type_list<int, long, double, char>, 3>, type_list<int, long, double>>, "list::remove: failed");
    static_assert(std::is_same_v<list::remove<type_list<int, long, double, char>, 4>, type_list<int, long, double, char>>, "list::remove: failed");
    static_assert(std::is_same_v<list::remove<type_list<int, long, double, char>, 40>, type_list<int, long, double, char>>, "list::remove: failed");
    static_assert(std::is_same_v<list::remove<type_list<>, 0>, type_list<>>, "list::remove: failed");
    static_assert(std::is_same_v<list::remove<type_list<>, 10>, type_list<>>, "list::remove: failed");

    static_assert(std::is_same_v<list::remove_type<type_list<int, int, int, int, type_list<int, int>>, int>, type_list<type_list<int, int>>>, "list::remove_type: failed");
    static_assert(std::is_same_v<list::remove_type<type_list<int, int, int, int>, int>, type_list<>>, "list::remove_type: failed");
    static_assert(std::is_same_v<list::remove_type<type_list<int, int, char, int, int>, int>, type_list<char>>, "list::remove_type: failed");
    static_assert(std::is_same_v<list::remove_type<type_list<char, int, int, int, int>, int>, type_list<char>>, "list::remove_type: failed");
    static_assert(std::is_same_v<list::remove_type<type_list<int, int, int, int, char>, int>, type_list<char>>, "list::remove_type: failed");
    static_assert(std::is_same_v<list::remove_type<type_list<char, int, int, int, int, char>, int>, type_list<char, char>>, "list::remove_type: failed");
    static_assert(std::is_same_v<list::remove_type<type_list<char, int, int, char, int, int, char>, int>, type_list<char, char, char>>, "list::remove_type: failed");
    static_assert(std::is_same_v<list::remove_type<type_list<char, char, char>, int>, type_list<char, char, char>>, "list::remove_type: failed");
    static_assert(std::is_same_v<list::remove_type<type_list<>, int>, type_list<>>, "list::remove_type: failed");

    static_assert(std::is_same_v<list::filter<type_list<char, int, int, char, int, int, char>, std::is_floating_point>, type_list<>>, "list::filter: failed");
    static_assert(std::is_same_v<list::filter<type_list<char, int, int, double, int, int, char>, std::is_floating_point>, type_list<double>>, "list::filter: failed");
    static_assert(std::is_same_v<list::filter<type_list<float, char, int, int, double, int, int, char>, std::is_floating_point>, type_list<float, double>>, "list::filter: failed");
    static_assert(std::is_same_v<list::filter<type_list<float, char, int, int, double, int, int, char, long double>, std::is_floating_point>, type_list<float, double, long double>>, "list::filter: failed");

    static_assert(std::is_same_v<list::reverse<type_list<>>, type_list<>>, "list::reverse: failed");
    static_assert(std::is_same_v<list::reverse<type_list<int>>, type_list<int>>, "list::reverse: failed");
    static_assert(std::is_same_v<list::reverse<type_list<int, int>>, type_list<int, int>>, "list::reverse: failed");
    static_assert(std::is_same_v<list::reverse<type_list<float, int>>, type_list<int, float>>, "list::reverse: failed");
    static_assert(std::is_same_v<list::reverse<type_list<double, float, int>>, type_list<int, float, double>>, "list::reverse: failed");
}
#endif
