#pragma once

#include "../hash/hash.hpp"
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace pride::ct
{
    namespace internal::fnv1a
    {
        // See http://www.isthe.com/chongo/tech/comp/fnv/#FNV-param
        template<typename Hash>
        struct constant {};

        template<>
        struct constant<uint32_t>
        {
            static constexpr uint32_t prime = 16777619ull;
            static constexpr uint32_t offset = 2166136261ull;
        };

        template<>
        struct constant<uint64_t>
        {
            static constexpr uint64_t prime = 1099511628211ull;
            static constexpr uint64_t offset = 14695981039346656037ull;
        };

        template<typename Char>
        constexpr hash32_t compute32(const Char* key, hash32_t h = constant<hash32_t>::offset)
        {
            return !*key ? h : compute32(key + 1, static_cast<hash32_t>(1ULL * (h ^ *key) * constant<hash32_t>::prime));
        }

        constexpr uint64_t lo(uint64_t x) { return x & uint32_t(-1); };
        constexpr uint64_t hi(uint64_t x) { return x >> 32; };

        // https://stackoverflow.com/a/37659510
        constexpr uint64_t mulu64( uint64_t a, uint64_t b )
        {
            return 0
                + (lo( a )*lo( b ) & uint32_t(-1))
                +   (
                        (
                            (
                                (
                                    (
                                        hi( lo( a )*lo( b ) ) +
                                        lo( a )*hi( b )
                                    )
                                    & uint32_t(-1)
                                )
                                + hi( a )*lo( b )
                            )
                            & uint32_t(-1)
                        )
                        << 32
                    );
        }

        template<typename Char>
        constexpr hash64_t compute64(const Char* key, hash64_t h = constant<hash64_t>::offset)
        {
           return !*key ? h : compute64(key + 1, mulu64(h ^ *key, constant<hash64_t>::prime));
        }

        template<typename Hash, typename Char>
        constexpr Hash fnv1a(const Char* input, size_t len)
        {
            static_assert(std::is_same<Char, char>() || std::is_same<Char, wchar_t>(), "Input type is not supported");
            static_assert(std::is_same<Hash, hash64_t>() || std::is_same<Hash, hash32_t>(), "Hash type is not supported");
            if constexpr(std::is_same<Hash, hash64_t>::value)
                return compute64(input);
            else
                return compute32(input);
        }
    }

    // ─── HASH ───────────────────────────────────────────────────────────────────────

    template<typename Char, size_t N>
    constexpr hash_t fnv1a(const Char(&input)[N])
    {
        return internal::fnv1a::fnv1a<hash_t>(input, N - 1);
    }

    template<typename Char>
    constexpr hash_t fnv1a(const Char* input, size_t len)
    {
        return internal::fnv1a::fnv1a<hash_t>(input, len);
    }

    // ─── HASH 32 ────────────────────────────────────────────────────────────────────

    template<typename Char, size_t N>
    constexpr hash32_t fnv1a32(const Char(&input)[N])
    {
        return internal::fnv1a::fnv1a<hash32_t>(input, N - 1);
    }

    template<typename Char>
    constexpr hash32_t fnv1a32(const Char* input, size_t len)
    {
        return internal::fnv1a::fnv1a<hash32_t>(input, len);
    }

    // ─── HASH 64 ────────────────────────────────────────────────────────────────────

    template<typename Char, size_t N>
    constexpr hash64_t fnv1a64(const Char(&input)[N])
    {
        return internal::fnv1a::fnv1a<hash64_t>(input, N - 1);
    }

    template<typename Char>
    constexpr hash64_t fnv1a64(const Char* input, size_t len)
    {
        return internal::fnv1a::fnv1a<hash64_t>(input, len);
    }
} // namespace pride::ct

// ─── HASH ───────────────────────────────────────────────────────────────────────

constexpr pride::hash_t operator "" _fnv1a(const char* ptr, const size_t len)
{
    return ::pride::ct::internal::fnv1a::fnv1a<pride::hash_t>(ptr, len);
}

constexpr pride::hash_t operator "" _fnv1a(const wchar_t* ptr, const size_t len)
{
    return ::pride::ct::internal::fnv1a::fnv1a<pride::hash_t>(ptr, len);
}

// ─── HASH 32 ────────────────────────────────────────────────────────────────────

constexpr pride::hash32_t operator "" _fnv1a32(const char* ptr, const size_t len)
{
    return ::pride::ct::internal::fnv1a::fnv1a<pride::hash32_t>(ptr, len);
}

constexpr pride::hash32_t operator "" _fnv1a32(const wchar_t* ptr, const size_t len)
{
    return ::pride::ct::internal::fnv1a::fnv1a<pride::hash32_t>(ptr, len);
}

// ─── HASH 64 ────────────────────────────────────────────────────────────────────

constexpr pride::hash64_t operator "" _fnv1a64(const char* ptr, const size_t len)
{
    return ::pride::ct::internal::fnv1a::fnv1a<pride::hash64_t>(ptr, len);
}

constexpr pride::hash64_t operator "" _fnv1a64(const wchar_t* ptr, const size_t len)
{
    return ::pride::ct::internal::fnv1a::fnv1a<pride::hash64_t>(ptr, len);
}

// ────────────────────────────────────────────────────────────────────────────────

#if defined(PRI_USE_CT_TEST)
    static_assert(pride::ct::fnv1a("This should") != pride::ct::fnv1a("not match"));
    static_assert(pride::ct::fnv1a("This should") != "not match"_fnv1a);
    static_assert(pride::ct::fnv1a("This should match") == "This should match"_fnv1a);
#endif
