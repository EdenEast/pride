
#pragma once

#include "../config/detection/compiler.hpp"
#include "string.hpp"
#include "hash.hpp"
#include <array>
#include <utility>

#if defined(PRI_COMPILER_MSVC)
    #define PRI_PRETTY_FUNCTION __FUNCSIG__
    #define PRI_FIND(s) s.crop(s.find("::tid<") + 6, pride::ct::strlen(">::type_name(void) noexcept"))

    #pragma warning( push )
    #pragma warning( disable : 4307)
#else
    #define PRI_PRETTY_FUNCTION __PRETTY_FUNCTION__
    #define PRI_FIND(s) s.crop(s.find("= ") + 2, pride::ct::strlen("]"))
#endif

namespace pride::ct
{
    namespace internal
    {
        template<typename Type, size_t... Idxs>
        constexpr std::array<char, sizeof...(Idxs) + 1> make_id_array(std::index_sequence<Idxs...>&&) { return {{Type::str[Idxs]..., '\0'}}; }

        template<typename Type, size_t Count = Type::size>
        constexpr auto make_id_array() { return make_id_array<Type>(std::make_index_sequence<Count>()); }

        template<typename Type>
        struct tid
        {
            static constexpr string type_name() noexcept
            {
                constexpr string s = { PRI_PRETTY_FUNCTION };
                constexpr string t = PRI_FIND(s);
                return t;
            };

            static constexpr const char* str = type_name().str;
            static constexpr size_t size = type_name().size;
            static constexpr hash_t hash = type_name().hash();
        };

        // template<auto Value>
        // struct vid
        // {
        //     static constexpr string type_name() noexcept
        //     {
        //         constexpr string s = { PRI_PRETTY_FUNCTION };
        //         constexpr string t = PRI_FIND(s);
        //         return t;
        //     }

        //     static constexpr const char* str = type_name().str;
        //     static constexpr size_t size = type_name().size;
        //     static constexpr hash_t hash = type_name().hash();
        // };

        template<typename T>
        struct tholder { static constexpr auto name = internal::make_id_array<internal::tid<T>>(); };
        // template<auto V>
        // struct vholder { static constexpr auto name = internal::make_id_array<internal::vid<V>>(); };
    }

    template<typename T>
    static constexpr string type_name = { internal::tholder<T>::name.data(), internal::tholder<T>::name.size() - 1 }; // internal::tid<T>::type_name();

    // template<auto V>
    // static constexpr string value_name = { internal::vholder<V>::name.data(), internal::vholder<V>::name.size() - 1 }; // internal::vid<V>::type_name();

    template<typename T>
    static constexpr string decay_type_name = type_name<std::decay_t<T>>;

    template<typename T>
    static constexpr hash_t type_hash = { pride::ct::fnv1a(internal::tholder<T>::name.data(), internal::tholder<T>::name.size() - 1) }; // internal::tid<T>::hash;

    // template<auto V>
    // static constexpr hash_t value_hash = { hash::fnv1a(internal::vholder<V>::name.data(),internal::vholder<V>::name.size() - 1) }; // internal::tid<T>::hash;

    template<typename T>
    static constexpr hash_t decay_type_hash = type_hash<std::decay_t<T>>;
}

#undef PRI_PRETTY_FUNCTION
#undef PRI_FIND

#if defined(PRI_USE_CT_TEST)
    static_assert(pride::ct::type_name<int> == pride::ct::string{"int"});
    static_assert(pride::ct::type_name<float> == pride::ct::string{"float"});
    static_assert(pride::ct::decay_type_name<const int&> == pride::ct::type_name<int>);
#endif

#if defined(PRI_COMPILER_MSVC)
    #pragma warning( pop )
#endif
