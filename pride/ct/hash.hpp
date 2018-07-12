#pragma once

#include "../hash/hash.hpp"
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace pride::ct
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
            static constexpr uint64_t prime = 1099511628211ull;
            static constexpr uint64_t offset = 14695981039346656037ull;
        };

        template<typename Hash, typename Char>
        constexpr Hash compute(const Char* key, size_t len)
        {
            auto h = constant<Hash>::offset;
            for (size_t i = 0; i < len; ++i)
            {
                h ^= static_cast<uint8_t>(key[i]);
                h *= constant<Hash>::prime;
            }
            return static_cast<Hash>(h);
        }

        template<typename Hash, typename Char>
        constexpr Hash fnv1a(const Char* input, size_t len)
        {
            static_assert(std::is_same<Char, char>() || std::is_same<Char, wchar_t>(), "Input type is not supported");
            static_assert(std::is_same<Hash, hash64_t>() || std::is_same<Hash, hash32_t>(), "Hash type is not supported");
            return compute<Hash>(input, len);
        }
    }

    // ─── HASH ───────────────────────────────────────────────────────────────────────

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

    // ─── HASH 32 ────────────────────────────────────────────────────────────────────

    template<typename Char, size_t N>
    constexpr hash32_t fnv1a32(const Char(&input)[N])
    {
        return detail::fnv1a::fnv1a<hash32_t>(input, N - 1);
    }

    template<typename Char>
    constexpr hash32_t fnv1a32(const Char* input, size_t len)
    {
        return detail::fnv1a::fnv1a<hash32_t>(input, len);
    }

    // ─── HASH 64 ────────────────────────────────────────────────────────────────────

    template<typename Char, size_t N>
    constexpr hash64_t fnv1a64(const Char(&input)[N])
    {
        return detail::fnv1a::fnv1a<hash64_t>(input, N - 1);
    }

    template<typename Char>
    constexpr hash64_t fnv1a64(const Char* input, size_t len)
    {
        return detail::fnv1a::fnv1a<hash64_t>(input, len);
    }
} // namespace pride::ct

// ─── HASH ───────────────────────────────────────────────────────────────────────

constexpr pride::hash_t operator "" _fnv1a(const char* ptr, const size_t len)
{
    return ::pride::ct::detail::fnv1a::fnv1a<pride::hash_t>(ptr, len);
}

constexpr pride::hash_t operator "" _fnv1a(const wchar_t* ptr, const size_t len)
{
    return ::pride::ct::detail::fnv1a::fnv1a<pride::hash_t>(ptr, len);
}

// ─── HASH 32 ────────────────────────────────────────────────────────────────────

constexpr pride::hash32_t operator "" _fnv1a32(const char* ptr, const size_t len)
{
    return ::pride::ct::detail::fnv1a::fnv1a<pride::hash32_t>(ptr, len);
}

constexpr pride::hash32_t operator "" _fnv1a32(const wchar_t* ptr, const size_t len)
{
    return ::pride::ct::detail::fnv1a::fnv1a<pride::hash32_t>(ptr, len);
}

// ─── HASH 64 ────────────────────────────────────────────────────────────────────

constexpr pride::hash64_t operator "" _fnv1a64(const char* ptr, const size_t len)
{
    return ::pride::ct::detail::fnv1a::fnv1a<pride::hash64_t>(ptr, len);
}

constexpr pride::hash64_t operator "" _fnv1a64(const wchar_t* ptr, const size_t len)
{
    return ::pride::ct::detail::fnv1a::fnv1a<pride::hash64_t>(ptr, len);
}

// ────────────────────────────────────────────────────────────────────────────────

#if defined(PRI_USE_CT_TEST)
    static_assert(pride::ct::fnv1a("This should") != pride::ct::fnv1a("not match"));
    static_assert(pride::ct::fnv1a("This should") != "not match"_fnv1a);
    static_assert(pride::ct::fnv1a("This should match") == "This should match"_fnv1a);
#endif
