
#pragma once

#include <cstddef>
#include <cstdint>

namespace pride { namespace ct
{
    using hash_t = std::uint64_t;

    namespace detail
    {
        // See http://www.isthe.com/chongo/tech/comp/fnv/#FNV-param
        constexpr hash_t fvn_basis = 14695981039346656037ull;
        constexpr hash_t fnv_prime = 1099511628211ull;
    }

    constexpr hash_t fnv1a_hash(std::size_t n, const char* str, hash_t hash = detail::fvn_basis)
    {
        return n > 0 ? fnv1a_hash(n - 1, str + 1, (hash ^ *str) * detail::fnv_prime) : hash;
    }

    template<std::size_t N>
    constexpr hash_t fnv1a_hash(const char (&array)[N])
    {
        return (fnv1a_hash(N - 1, &array[0]));
    }

#if defined(PRI_USE_CT_TEST)

static_assert(fnv1a_hash("is the same") == fnv1a_hash("is the same"));
static_assert(fnv1a_hash("this is") != fnv1a_hash("not the same"));

#endif

}}
