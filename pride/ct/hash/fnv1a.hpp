#pragma once

#include <cstdint>
#include <type_traits>

namespace pride::ct::hash
{
    namespace detail::fnv1a
    {
        // See http://www.isthe.com/chongo/tech/comp/fnv/#FNV-param
        template<typename Hash>
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

        template<typename Hash, typename Char>
        constexpr Hash compute(const Char input, const Hash prime, Hash hash)
        {
            hash ^= input;
            hash *= prime;
            return hash;
        }

        template<typename Hash, typename Char, size_t N>
        constexpr Hash process(const Char(&input)[N], const Hash prime, Hash hash)
        {
            return process(input, N, prime, hash);
        }

        template<typename Hash, typename Char>
        constexpr Hash process(const Char* input, const size_t len, const Hash prime, Hash hash)
        {
            for (size_t i = 0; i < len; ++i)
                hash = compute(input[i], prime, hash);
            return hash;
        }

        template<typename Hash, typename Char>
        constexpr Hash fnv1a(const Char* input, size_t len)
        {
            static_assert(std::is_same<Char, char>() || std::is_same<Char, wchar_t>(), "Input type is not supported");
            static_assert(std::is_same<Hash, hash64_t>() || std::is_same<Hash, hash32_t>(), "Hash type is not supported");
            return process<Hash>(input, len, constant<Hash>::prime, constant<Hash>::offset);
        }
    }

    // hash_t  --------------------------------------------------------------

    template<typename Char, size_t N>
    constexpr hash_t fnv1a(const Char(&input)[N])
    {
        return detail::fnv1a::fnv1a<hash_t>(input, N - 1);
    }

    template<typename Char>
    constexpr hash_t fnv1a(const Char* input, size_t len)
    {
        return detail::fnv1a::fnv1a<hash_t>(input, len);
    }

    // hash32_t  --------------------------------------------------------------

    template<typename Char, size_t N>
    constexpr hash32_t fnv1a32(const Char(&input)[N])
    {
        return detail::fnv1a::fnv1a<hash32_t>(input, N);
    }

    template<typename Char>
    constexpr hash32_t fnv1a32(const Char* input, size_t len)
    {
        return detail::fnv1a::fnv1a<hash32_t>(input, len);
    }

    // hash64_t  --------------------------------------------------------------

    template<typename Char, size_t N>
    constexpr hash64_t fnv1a64(const Char(&input)[N])
    {
        return detail::fnv1a::fnv1a<hash64_t>(input, N);
    }

    template<typename Char>
    constexpr hash64_t fnv1a64(const Char* input, size_t len)
    {
        return detail::fnv1a::fnv1a<hash64_t>(input, len);
    }

    // ----------------------------------------------------------------------
}

constexpr pride::hash_t operator "" _fnv1a(const char* ptr, const size_t len)
{
    return ::pride::ct::hash::detail::fnv1a::fnv1a<pride::hash32_t>(ptr, len);
}

constexpr pride::hash_t operator "" _fnv1a(const wchar_t* ptr, const size_t len)
{
    return ::pride::ct::hash::detail::fnv1a::fnv1a<pride::hash32_t>(ptr, len);
}

constexpr pride::hash32_t operator "" _fnv1a32(const char* ptr, const size_t len)
{
    return ::pride::ct::hash::detail::fnv1a::fnv1a<pride::hash32_t>(ptr, len);
}

constexpr pride::hash32_t operator "" _fnv1a32(const wchar_t* ptr, const size_t len)
{
    return ::pride::ct::hash::detail::fnv1a::fnv1a<pride::hash32_t>(ptr, len);
}

constexpr pride::hash64_t operator "" _fnv1a64(const char* ptr, const size_t len)
{
    return ::pride::ct::hash::detail::fnv1a::fnv1a<pride::hash64_t>(ptr, len);
}

constexpr pride::hash64_t operator "" _fnv1a64(const wchar_t* ptr, const size_t len)
{
    return ::pride::ct::hash::detail::fnv1a::fnv1a<pride::hash64_t>(ptr, len);
}
