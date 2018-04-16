
#pragma once

#include <cstddef>
#include <cstdint>

namespace pride { namespace ct
{
    using hash_t = std::uint64_t;

    namespace hash
    {
        namespace detail
        {
            // See http://www.isthe.com/chongo/tech/comp/fnv/#FNV-param
            constexpr hash_t fvn_basis = 14695981039346656037ull;
            constexpr hash_t fnv_prime = 1099511628211ull;
        }

        constexpr hash_t fnv1a(std::size_t n, const char* str, hash_t hash = detail::fvn_basis)
        {
            return n > 0 ? fnv1a(n - 1, str + 1, (hash ^ *str) * detail::fnv_prime) : hash;
        }

        template<std::size_t N>
        constexpr hash_t fnv1a(const char (&array)[N])
        {
            return (fnv1a(N - 1, &array[0]));
        }
    }
#define PRI_USE_CT_TEST
#if defined(PRI_USE_CT_TEST)

static_assert(hash::fnv1a("is the same") == hash::fnv1a("is the same"), "cstring == operator failed");
static_assert(hash::fnv1a("this is") != hash::fnv1a("not the same"), "cstring != operator failed");

#endif

}}
