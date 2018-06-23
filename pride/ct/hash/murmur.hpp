// https://en.wikipedia.org/wiki/MurmurHash
#pragma once

#include "hash.hpp"
#include <cstddef>
#include <cstdint>
#include <stdexcept>

namespace pride::hash
{
    namespace detail::mm3
    {
        static constexpr size_t strlen(const char* const str)
        {
            return str[0] ? strlen(str + 1) + 1 : 0;
        }

        struct chunker
        {
            template<size_t N>
            constexpr chunker(const char(&a)[N]) : data(a), size(N - 1) {}
            constexpr chunker(const char* a, size_t len) : data(a), size(len) {}

            constexpr char operator[](size_t n) const
            {
                return n < size ? data[n] : throw std::out_of_range("");
            }

            constexpr uint32_t get_block(size_t idx)
            {
                uint32_t i = (block_size() + idx) * 4;
                uint32_t b0 = data[i];
                uint32_t b1 = data[i + 1];
                uint32_t b2 = data[i + 2];
                uint32_t b3 = data[i + 3];
                return (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
            }

            constexpr size_t block_size() const { return size / 4; }

            constexpr char tail(const size_t n) const
            {
                size_t tail_size = size % 4;
                return data[size - tail_size + n];
            }

            const char* data;
            size_t size;
        };

        constexpr uint32_t mm3_32(const char* key, uint32_t seed)
        {
            detail::mm3::chunker chunk(key, detail::mm3::strlen(key));
            uint32_t h1 = seed;

            constexpr const uint32_t c1 = 0xcc9e2d51;
            constexpr const uint32_t c2 = 0x1b873593;

            const int nblocks = chunk.block_size();
            for (int i = -nblocks; i; ++i)
            {
                uint32_t k1 = chunk.get_block(i);

                k1 *= c1;
                k1 = (k1 << 15) | (k1 >> (32 - 15));
                k1 *= c2;

                h1 ^= k1;
                h1 = (h1 << 13) | (h1 >> (32 - 13));
                h1 = h1 * 5 + 0xe6546b64;
            }

            uint32_t k1 = 0;

            switch (chunk.size & 3)
            {
            case 3:
                k1 ^= chunk.tail(2) << 16;
                break;
            case 2:
                k1 ^= chunk.tail(1) << 8;
                break;
            case 1:
                k1 ^= chunk.tail(0);
                k1 *= c1;
                k1 = (k1 << 15) | (k1 >> (32 - 15));
                k1 *= c2;
                h1 ^= k1;
                break;
            };

            h1 ^= h1 >> 16;
            h1 *= 0x85ebca6b;
            h1 ^= h1 >> 13;
            h1 *= 0xc2b2ae35;
            h1 ^= h1 >> 16;

            return h1;
        }
    }

    constexpr uint32_t mm3_32(const char* key, uint32_t seed = 0x9747b28c)
    {
        return detail::mm3::mm3_32(key, seed);
    }
}

constexpr uint32_t operator "" _mm3_32(const char* ptr, const size_t len)
{
    return ::pride::hash::mm3_32(ptr);
}
