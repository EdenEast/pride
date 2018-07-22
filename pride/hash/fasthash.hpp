
#pragma once

#include "hash.hpp"
#include <cstddef>

namespace pride::hash
{
    namespace internal::fasthash
    {
        template<typename Hash>
        struct constant;
        template<>struct constant<hash32_t> { static constexpr hash32_t seed = 0xE9481AFC; };
        template<>struct constant<hash64_t> { static constexpr hash64_t seed = 0x7483b72ab337ef62ULL; };

        // Compression function for a Merkle-Damgard construction.
        static inline uint64_t mix(uint64_t h)
        {
            h ^= h >> 23;
            h *= 0x2127599bf4325c37ULL;
            h ^= h >> 47;
            return h;
        }

        template<typename Hash>
        Hash compute(const void* data, size_t len, Hash seed);

        template<>
        inline uint64_t compute<uint64_t>(const void* data, size_t len, uint64_t seed)
        {
            const uint64_t    m = 0x880355f21e6d1965ULL;
            const uint64_t *pos = (const uint64_t *)data;
            const uint64_t *end = pos + (len / 8);
            const unsigned char *pos2;
            uint64_t h = seed ^ (len * m);
            uint64_t v;

            while (pos != end)
            {
                v  = *pos++;
                h ^= mix(v);
                h *= m;
            }

            pos2 = (const unsigned char*)pos;
            v = 0;

            switch (len & 7)
            {
            case 7: v ^= (uint64_t)pos2[6] << 48; break;
            case 6: v ^= (uint64_t)pos2[5] << 40; break;
            case 5: v ^= (uint64_t)pos2[4] << 32; break;
            case 4: v ^= (uint64_t)pos2[3] << 24; break;
            case 3: v ^= (uint64_t)pos2[2] << 16; break;
            case 2: v ^= (uint64_t)pos2[1] << 8; break;
            case 1: v ^= (uint64_t)pos2[0]; break;
                h ^= mix(v);
                h *= m;
            }

            return mix(h);
        }

        template<>
        inline uint32_t compute<uint32_t>(const void* data, size_t len, uint32_t seed)
        {
            auto result = compute<uint64_t>(data, len, seed);
            return static_cast<uint32_t>(result - (result >> 32));
        }

        template<typename Hash, typename T>
        Hash fasthash(const T* data, size_t len, Hash seed)
        {
            return compute<Hash>(reinterpret_cast<const void*>(data), len, seed);
        }
    }

    template<typename T>
    hash_t fasthash(const T* data, size_t len, hash_t seed = internal::fasthash::constant<hash_t>::seed)
    {
        return internal::fasthash::fasthash<hash_t>(data, len, seed);
    }

    template<typename T>
    hash32_t fasthash32(const T* data, size_t len, hash32_t seed = internal::fasthash::constant<hash32_t>::seed)
    {
        return internal::fasthash::fasthash<hash32_t>(data, len, seed);
    }

    template<typename T>
    hash64_t fasthash64(const T* data, size_t len, hash64_t seed = internal::fasthash::constant<hash64_t>::seed)
    {
        return internal::fasthash::fasthash<hash64_t>(data, len, seed);
    } // namespace internal::fasthash
} // namespace pride::hash
