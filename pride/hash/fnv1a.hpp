
#pragma once

#include "hash.hpp"
#include <cstdint>

namespace pride::hash
{
    namespace internal::fnv1a
    {
        template<typename T>
        struct constant {};
        template<> struct constant<uint32_t>
        {
            static constexpr uint32_t seed = 0x6384BA69;
            static constexpr uint32_t prime = 16777619u;
            static constexpr uint32_t offset = 2166136261u;
        };
        template<> struct constant<uint64_t>
        {
            static constexpr uint64_t seed = 0xab72018e887f2172ULL;
            static constexpr uint64_t prime = 1099511628211uLL;
            static constexpr uint64_t offset = 14695981039346656037uLL;
        };

        template<typename Hash>
        Hash compute(const void* key, size_t len, Hash seed)
        {
            auto h = constant<Hash>::seed;
            const uint8_t* data = (const uint8_t*)key;

            h ^= constant<Hash>::offset;
            for (size_t i = 0; i < len; i++)
            {
                h ^= data[i];
                h *= constant<uint32_t>::prime;
            }

            return h;
        }

        template<typename Hash, typename T>
        Hash fnv1a(const T* data, size_t len, Hash seed)
        {
            return compute<Hash>(reinterpret_cast<const void*>(data), len, seed);
        }
    } // namespace internal::fnv1a

    template<typename T>
    hash_t fnv1a(const T* data, size_t len, hash_t seed = internal::fnv1a::constant<hash_t>::seed)
    {
        return internal::fnv1a::fnv1a<hash_t>(data, len, seed);
    }

    template<typename T>
    hash32_t fnv1a32(const T* data, size_t len, hash32_t seed = internal::fnv1a::constant<hash32_t>::seed)
    {
        return internal::fnv1a::fnv1a<hash32_t>(data, len, seed);
    }

    template<typename T>
    hash64_t fnv1a64(const T* data, size_t len, hash64_t seed = internal::fnv1a::constant<hash64_t>::seed)
    {
        return internal::fnv1a::fnv1a<hash64_t>(data, len, seed);
    }
} // namespace pride::hash
