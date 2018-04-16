
#pragma once

#include "../cstring.hpp"
#include "../hash.hpp"
#include "details/nameof.hpp"

namespace pride { namespace ct
{
    struct type_id_t
    {
        constexpr type_id_t() : _name("void") {}
        constexpr type_id_t(const cstring_t& name) : _name(name) {}

        type_id_t& operator=(const type_id_t&) = default;

        constexpr hash_t hash() const { return _name.hash(); }
        constexpr const cstring_t& name() const { return _name; }

        friend constexpr bool operator==(const type_id_t& lhs, const type_id_t& rhs)
        {
            return lhs.hash() == rhs.hash();
        }

        friend constexpr bool operator!=(const type_id_t& lhs, const type_id_t& rhs)
        {
            return !(lhs == rhs);
        }

    private:
        cstring_t _name;
    };

    struct unnaamed_type_id_t
    {
        constexpr unnaamed_type_id_t(const hash_t hash) : _hash(hash) {}
        constexpr unnaamed_type_id_t(const type_id_t& id) : _hash(id.hash()) {}

        unnaamed_type_id_t& operator=(const unnaamed_type_id_t&) = default;

        constexpr hash_t hash() const { return _hash; }

        friend constexpr bool operator==(const unnaamed_type_id_t& lhs, const unnaamed_type_id_t& rhs)
        {
            return lhs.hash() == rhs.hash();
        }

        friend constexpr bool operator!=(const unnaamed_type_id_t& lhs, const unnaamed_type_id_t& rhs)
        {
            return !(lhs == rhs);
        }

    private:
        hash_t _hash;
    };

    // This mimics std::type_index when using a map
    using type_index = unnaamed_type_id_t;

    namespace detail
    {
        template<std::size_t N>
        constexpr unnaamed_type_id_t id_from_name(const char(&type_name)[N])
        {
            return hash::fnv1a(type_name);
        }

        constexpr unnaamed_type_id_t id_from_name(const char* type_name, std::size_t length)
        {
            return hash::fnv1a(length, type_name);
        }

        constexpr unnaamed_type_id_t id_from_name(const cstring_t type_name)
        {
            return hash::fnv1a(type_name.length(), type_name.begin());
        }

        inline unnaamed_type_id_t id_from_name(const std::string& str)
        {
            return hash::fnv1a(str.size(), str.data());
        }
    }

    namespace detail
    {
         template<typename T>
         constexpr type_id_t type_id()
         {
             return { pride::ct::nameof<T>() };
         }

         template<typename T>
         constexpr unnaamed_type_id_t unnaamed_type_id()
         {
             return { detail::id_from_name(::pride::ct::nameof<T>()) };
         }
    }

    // Returns the type information at compile time (ct) for type T
    template<typename T>
    constexpr type_id_t type_id()
    {
        return detail::type_id<T>();
    }

    // Returns type information at compile time (ct) for a value
    // Note: std::decay is applied to the type. use type_id<decltype(arg)>()
    // to preserve references and other information.
    template<typename T>
    constexpr type_id_t type_id(T&&)
    {
        return detail::type_id<typename std::decay<T>::type>();
    }

    // Returns the type information at compile time (ct) for type T
    template<typename T>
    constexpr unnaamed_type_id_t unnaamed_type_id()
    {
        return detail::unnaamed_type_id<T>();
    }

    // Returns unnammed type information at compile time (ct) for a value
    // Note: std::decay is applied to the type. use unnaamed_type_id<decltype(arg)>()
    // to preserve references and other information.
    template<typename T>
    constexpr unnaamed_type_id_t unnaamed_type_id(T&&)
    {
        return detail::unnaamed_type_id<typename std::decay<T>::type>();
    }
}}
