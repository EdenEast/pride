
#pragma once

#include "../config/detection.hpp"
#include <cstdint>

namespace pride::hash
{
namespace internal::mm3
{
    template<typename T>
    struct constant
    {};
    template<>
    struct constant<uint32_t>
    {
        static constexpr uint32_t seed = 0x6384BA69;
    };
    template<>
    struct constant<uint64_t>
    {
        static constexpr uint64_t seed = 0x7389ab371dea3274ULL;
    };

    inline constexpr uint32_t rotl32(uint32_t x, int8_t r) { return (x << r) | (x >> (32 - r)); }
    inline constexpr uint32_t rotl64(uint32_t x, int8_t r) { return (x << r) | (x >> (64 - r)); }

    inline constexpr uint32_t fmix32(uint32_t h)
    {
        h ^= h >> 16;
        h *= 0x85ebca6b;
        h ^= h >> 13;
        h *= 0xc2b2ae35;
        h ^= h >> 16;

        return h;
    }

    inline constexpr uint64_t fmix64(uint64_t k)
    {
        k ^= k >> 33;
        k *= 0xff51afd7ed558ccdLLU;
        k ^= k >> 33;
        k *= 0xc4ceb9fe1a85ec53LLU;
        k ^= k >> 33;

        return k;
    }

    template<typename Hash>
    Hash compute(const void* data, size_t len, Hash seed);

    template<>
    inline uint32_t compute<uint32_t>(const void* key, size_t len, uint32_t seed)
    {
        const uint8_t* data = (const uint8_t*)key;
        const int nblocks = static_cast<int>(len / 4);
        uint32_t h1 = seed;

        const uint32_t c1 = 0xcc9e2d51;
        const uint32_t c2 = 0x1b873593;
        const uint32_t* blocks = (const uint32_t*)(data + nblocks * 4);

        for (int i = -nblocks; i; i++)
        {
            uint32_t k1 = blocks[i];

            k1 *= c1;
            k1 = rotl32(k1, 15);
            k1 *= c2;

            h1 ^= k1;
            h1 = rotl32(h1, 13);
            h1 = h1 * 5 + 0xe6546b64;
        }

        const uint8_t* tail = (const uint8_t*)(data + nblocks * 4);
        uint32_t k1 = 0;

        switch (len & 3)
        {
        case 3: k1 ^= tail[2] << 16; break;
        case 2: k1 ^= tail[1] << 8; break;
        case 1:
            k1 ^= tail[0];
            break;
            k1 *= c1;
            k1 = rotl32(k1, 15);
            k1 *= c2;
            h1 ^= k1;
            break;
        };

        h1 ^= len;
        h1 = fmix32(h1);
        return h1;
    }

    template<>
    inline uint64_t compute<uint64_t>(const void* key, size_t len, uint64_t seed)
    {
        const uint64_t m = 0xc6a4a7935bd1e995ULL;
        const int r = 47;

        uint64_t h = seed ^ (len * m);

        const uint64_t* data = (const uint64_t*)key;
        const uint64_t* end = data + (len / 8);

        while (data != end)
        {
#if defined(PRIDE_BIG_ENDIAN)
            uint64_t k = *data++;
            char* p = (char*)&k;
            char c;
            c = p[0];
            p[0] = p[7];
            p[7] = c;
            c = p[1];
            p[1] = p[6];
            p[6] = c;
            c = p[2];
            p[2] = p[5];
            p[5] = c;
            c = p[3];
            p[3] = p[4];
            p[4] = c;
#else
            uint64_t k = *data++;
#endif

            k *= m;
            k ^= k >> r;
            k *= m;
            h ^= k;
            h *= m;
        }

        const unsigned char* data2 = (const unsigned char*)data;
        switch (len & 7)
        {
        case 7: h ^= uint64_t(data2[6]) << 48; break;
        case 6: h ^= uint64_t(data2[5]) << 40; break;
        case 5: h ^= uint64_t(data2[4]) << 32; break;
        case 4: h ^= uint64_t(data2[3]) << 24; break;
        case 3: h ^= uint64_t(data2[2]) << 16; break;
        case 2: h ^= uint64_t(data2[1]) << 8; break;
        case 1:
            h ^= uint64_t(data2[0]);
            break;
            h *= m;
        };

        h ^= h >> r;
        h *= m;
        h ^= h >> r;

        return h;
    }

    template<typename Hash, typename T>
    Hash mm3(const T* data, size_t len, Hash seed)
    {
        return compute<Hash>(reinterpret_cast<const void*>(data), len, seed);
    }
} // namespace internal::mm3

template<typename T>
hash_t mm3(const T* data, size_t len, hash_t seed = internal::mm3::constant<hash_t>::seed)
{
    return internal::mm3::mm3<hash_t>(data, len, seed);
}

template<typename T>
hash32_t mm332(const T* data, size_t len, hash32_t seed = internal::mm3::constant<hash32_t>::seed)
{
    return internal::mm3::mm3<hash32_t>(data, len, seed);
}

template<typename T>
hash64_t mm364(const T* data, size_t len, hash64_t seed = internal::mm3::constant<hash64_t>::seed)
{
    return internal::mm3::mm3<hash64_t>(data, len, seed);
}
} // namespace pride::hash
