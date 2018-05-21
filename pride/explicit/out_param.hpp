
#pragma once

#include <type_traits>

namespace pride
{
    namespace detail
    {
        template<typename T>
        struct out_param
        {
            static_assert(std::is_lvalue_reference<T>::value, "Only references to non-const values are allowed");
            static_assert(!std::is_const<typename std::remove_reference<T>::type>::value, "Only references to non-const value are allowed");
            explicit out_param(T& ref) : ref(ref){}
            T& get() { return ref; }
            T& ref;
        };

        template<typename T>
        out_param<T> out(T& ref)
        {
            return out_param<T>(ref);

        }
    }

    using detail::out_param;
    using detail::out;
}
