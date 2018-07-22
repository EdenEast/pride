
#pragma once

#include "../config/detection.hpp"
#include "hash.hpp"
#include <array>
#include <cstdint>
#include <cstddef>

#if defined(PRI_COMPILER_MSVC)
#   define rotl32(x, r) _rotl(x, r)
#   define rotl64(x, r) _rotl64(x, r)
#else
#   define rotl32(x, r) ((x << r) | (x >> (32 - r)))
#   define rotl64(x, r) ((x << r) | (x >> (64 - r)))
#endif

namespace pride::hash
{
    namespace internal::xxhash
    {
        static inline uint32_t swap32(uint32_t x)
        {
            return  ((x << 24) & 0xff000000 ) |
                    ((x <<  8) & 0x00ff0000 ) |
                    ((x >>  8) & 0x0000ff00 ) |
                    ((x >> 24) & 0x000000ff );
        }

        static inline uint64_t swap64(uint64_t x)
        {
            return  ((x << 56) & 0xff00000000000000ULL) |
                    ((x << 40) & 0x00ff000000000000ULL) |
                    ((x << 24) & 0x0000ff0000000000ULL) |
                    ((x << 8)  & 0x000000ff00000000ULL) |
                    ((x >> 8)  & 0x00000000ff000000ULL) |
                    ((x >> 24) & 0x0000000000ff0000ULL) |
                    ((x >> 40) & 0x000000000000ff00ULL) |
                    ((x >> 56) & 0x00000000000000ffULL);
        }

        template<typename T>
        struct constant {};

        template<>
        struct constant<uint32_t>
        {
            constexpr static std::array<uint32_t, 5> primes {{
                2654435761U, 2246822519U, 3266489917U, 668265263U, 374761393U
            }};
            static constexpr uint32_t seed = 0xBA88B743;
        };

        template<>
        struct constant<uint64_t>
        {
            constexpr static std::array<uint64_t, 5> primes {{
                11400714785074694791ULL, 14029467366897019727ULL, 1609587929392839161ULL, 9650029242287828579ULL, 2870177450012600261ULL
            }};
            static constexpr uint64_t seed = 0x2836a5de78b865f5ULL;
        };

        using const32 = constant<uint32_t>;
        using const64 = constant<uint32_t>;

        inline uint32_t read32(const void* ptr)
        {
            return pride::detection::is_little_endian() ?
                *(uint32_t*)ptr : swap32(*(uint32_t*)ptr);
        }

        inline uint64_t read64(const void* ptr)
        {
            return pride::detection::is_little_endian() ?
                *(uint64_t*)ptr : swap64(*(uint64_t*)ptr);
        }

        template<typename Hash>
        Hash compute(const void* data, size_t len, Hash seed);

        template<>
        inline uint32_t compute<uint32_t>(const void* input, size_t len, uint32_t seed)
        {
            const uint8_t* p = (const uint8_t*)input;
            const uint8_t* end = p + len;
            uint32_t h32;

            if (len>=16)
            {
                const uint8_t* const limit = end - 16;
                uint32_t v1 = seed + constant<uint32_t>::primes[0] + constant<uint32_t>::primes[1];
                uint32_t v2 = seed + constant<uint32_t>::primes[1];
                uint32_t v3 = seed + 0;
                uint32_t v4 = seed - constant<uint32_t>::primes[0];

                do
                {
                    v1 += read32(p) * constant<uint32_t>::primes[1];
                    v1 = rotl32(v1, 13);
                    v1 *= constant<uint32_t>::primes[0];
                    p+=4;
                    v2 += read32(p) * constant<uint32_t>::primes[1];
                    v2 = rotl32(v2, 13);
                    v2 *= constant<uint32_t>::primes[0];
                    p+=4;
                    v3 += read32(p) * constant<uint32_t>::primes[1];
                    v3 = rotl32(v3, 13);
                    v3 *= constant<uint32_t>::primes[0];
                    p+=4;
                    v4 += read32(p) * constant<uint32_t>::primes[1];
                    v4 = rotl32(v4, 13);
                    v4 *= constant<uint32_t>::primes[0];
                    p+=4;
                }
                while (p<=limit);

                h32 = rotl32(v1, 1) + rotl32(v2, 7) + rotl32(v3, 12) + rotl32(v4, 18);
            }
            else
            {
                h32  = seed + constant<uint32_t>::primes[4];
            }

            h32 += (uint32_t) len;

            while (p+4<=end)
            {
                h32 += read32(p) * constant<uint32_t>::primes[2];
                h32  = rotl32(h32, 17) * constant<uint32_t>::primes[3];
                p+=4;
            }

            while (p<end)
            {
                h32 += (*p) * constant<uint32_t>::primes[4];
                h32 = rotl32(h32, 11) * constant<uint32_t>::primes[0] ;
                p++;
            }

            h32 ^= h32 >> 15;
            h32 *= constant<uint32_t>::primes[1];
            h32 ^= h32 >> 13;
            h32 *= constant<uint32_t>::primes[2];
            h32 ^= h32 >> 16;

            return h32;
        }

        template<>
        inline uint64_t compute<uint64_t>(const void* input, size_t len, uint64_t seed)
        {
            const uint8_t* p = (const uint8_t*)input;
            const uint8_t* end = p + len;
            uint64_t h64;

            if (len>=32)
            {
                const uint8_t* const limit = end - 32;
                uint64_t v1 = seed + const64::primes[0] + const64::primes[1];
                uint64_t v2 = seed + const64::primes[1];
                uint64_t v3 = seed + 0;
                uint64_t v4 = seed - const64::primes[0];

                do
                {
                    v1 += read64(p) * const64::primes[1];
                    p+=8;
                    v1 = rotl64(v1, 31);
                    v1 *= const64::primes[0];
                    v2 += read64(p) * const64::primes[1];
                    p+=8;
                    v2 = rotl64(v2, 31);
                    v2 *= const64::primes[0];
                    v3 += read64(p) * const64::primes[1];
                    p+=8;
                    v3 = rotl64(v3, 31);
                    v3 *= const64::primes[0];
                    v4 += read64(p) * const64::primes[1];
                    p+=8;
                    v4 = rotl64(v4, 31);
                    v4 *= const64::primes[0];
                }
                while (p<=limit);

                h64 = rotl64(v1, 1) + rotl64(v2, 7) + rotl64(v3, 12) + rotl64(v4, 18);

                v1 *= const64::primes[1];
                v1 = rotl64(v1, 31);
                v1 *= const64::primes[0];
                h64 ^= v1;
                h64 = h64 * const64::primes[0] + const64::primes[3];

                v2 *= const64::primes[1];
                v2 = rotl64(v2, 31);
                v2 *= const64::primes[0];
                h64 ^= v2;
                h64 = h64 * const64::primes[0] + const64::primes[3];

                v3 *= const64::primes[1];
                v3 = rotl64(v3, 31);
                v3 *= const64::primes[0];
                h64 ^= v3;
                h64 = h64 * const64::primes[0] + const64::primes[3];

                v4 *= const64::primes[1];
                v4 = rotl64(v4, 31);
                v4 *= const64::primes[0];
                h64 ^= v4;
                h64 = h64 * const64::primes[0] + const64::primes[3];
            }
            else
            {
                h64  = seed + const64::primes[4];
            }

            h64 += (uint64_t) len;

            while (p + 8 <= end)
            {
                uint64_t k1 = read64(p);
                k1 *= const64::primes[1];
                k1 = rotl64(k1,31);
                k1 *= const64::primes[0];
                h64 ^= k1;
                h64 = rotl64(h64,27) * const64::primes[0] + const64::primes[3];
                p+=8;
            }

            if (p + 4 <= end)
            {
                h64 ^= (uint64_t)(read32(p)) * const64::primes[0];
                h64 = rotl64(h64, 23) * const64::primes[1] + const64::primes[2];
                p+=4;
            }

            while (p < end)
            {
                h64 ^= (*p) * const64::primes[4];
                h64 = rotl64(h64, 11) * const64::primes[0];
                p++;
            }

            h64 ^= h64 >> 33;
            h64 *= const64::primes[1];
            h64 ^= h64 >> 29;
            h64 *= const64::primes[2];
            h64 ^= h64 >> 32;

            return h64;
        }

        template<typename Hash, typename T>
        Hash xxhash(const T* data, size_t len, Hash seed)
        {
            return compute<Hash>(reinterpret_cast<const void*>(data), len, seed);
        }
    } // namespace internal::xxhash

    template<typename T>
    hash_t xxhash(const T* data, size_t len, hash_t seed = internal::xxhash::constant<hash_t>::seed)
    {
        return internal::xxhash::xxhash<hash_t>(data, len, seed);
    }

    template<typename T>
    hash32_t xxhash32(const T* data, size_t len, hash32_t seed = internal::xxhash::constant<hash32_t>::seed)
    {
        return internal::xxhash::xxhash<hash32_t>(data, len, seed);
    }

    template<typename T>
    hash64_t xxhash64(const T* data, size_t len, hash64_t seed = internal::xxhash::constant<hash64_t>::seed)
    {
        return internal::xxhash::xxhash<hash64_t>(data, len, seed);
    }
} // namespace pride::hash

#undef rotl32
#undef rotl64
