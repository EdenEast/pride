// https://github.com/stevefan1999/constexpr_fnv/blob/master/constexpr_fnv.hpp
// https://github.com/Kronuz/constexpr-phf/blob/master/hashes.hh
#pragma once

#include <cstdint>
#include <type_traits>

namespace pride::hash
{
    namespace detail::fnv1a
    {
        // See http://www.isthe.com/chongo/tech/comp/fnv/#FNV-param
        template<typename T>
        struct constant {};

        template<>
        struct constant<uint32_t>
        {
            static constexpr uint32_t prime = 16777619u;
            static constexpr uint32_t offset = 2166136261u;
        };

        template<>
        struct constant<uint64_t>
        {
            static constexpr uint64_t prime = 1099511628211uLL;
            static constexpr uint64_t offset = 14695981039346656037uLL;
        };

        template<typename T, typename U>
        constexpr T compute(const U input, const T prime, T hash)
        {
            hash ^= input;
            hash *= prime;
            return hash;
        }

        template<typename T, typename U, size_t N>
        constexpr T process(const U(&input)[N], const T prime, T hash)
        {
            return process(input, N, prime, hash);
        }

        template<typename T, typename U>
        constexpr T process(const U* input, const size_t len, const T prime, T hash)
        {
            for (size_t i = 0; i < len; ++i)
                hash = compute(input[i], prime, hash);
            return hash;
        }

        template<typename T, typename U, size_t N>
        constexpr T fnv1a(const U(&input)[N])
        {
            return fnv1a(input, N);
        }

        template<typename T, typename U>
        constexpr T fnv1a(const U* input, size_t len)
        {
            static_assert(std::is_same<U, char>() || std::is_same<U, wchar_t>(), "Input type is not supported");
            static_assert(std::is_same<T, uint64_t>() || std::is_same<T, uint32_t>(), "Hash type is not supported");
            return process<T>(input, len, constant<T>::prime, constant<T>::offset);
        }
    }

    template<typename T, size_t N>
    constexpr uint32_t fnv1a(const T(&input)[N])
    {
        return detail::fnv1a::fnv1a<uint32_t, T, N>(input);
    }

    template<typename T>
    constexpr uint32_t fnv1a(const T* input, size_t len)
    {
        return detail::fnv1a::fnv1a<uint32_t>(input, len);
    }

    template<typename T, size_t N>
    constexpr uint64_t fnv1a64(const T(&input)[N])
    {
        return detail::fnv1a::fnv1a<uint64_t, T, N>(input);
    }

    template<typename T>
    constexpr uint64_t fnv1a64(const T* input, size_t len)
    {
        return detail::fnv1a::fnv1a<uint64_t>(input, len);
    }
}

constexpr uint32_t operator "" _fnv1a_32(const char* ptr, const size_t len)
{
    return ::pride::hash::detail::fnv1a::fnv1a<uint32_t>(ptr, len);
}

constexpr uint32_t operator "" _fnv1a_32(const wchar_t* ptr, const size_t len)
{
    return ::pride::hash::detail::fnv1a::fnv1a<uint32_t>(ptr, len);
}

constexpr uint64_t operator "" _fnv1a_64(const char* ptr, const size_t len)
{
    return ::pride::hash::detail::fnv1a::fnv1a<uint64_t>(ptr, len);
}

constexpr uint64_t operator "" _fnv1a_64(const wchar_t* ptr, const size_t len)
{
    return ::pride::hash::detail::fnv1a::fnv1a<uint64_t>(ptr, len);
}
