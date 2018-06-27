#pragma once

#include <cstddef>
#include <cstdint>

namespace pride::ct::hash
{
    namespace detail::crc32
    {
        constexpr uint32_t CRC32_TABLE[] = {
            0x00000000, 0x1DB71064, 0x3B6E20C8, 0x26D930AC, 0x76DC4190, 0x6B6B51F4, 0x4DB26158, 0x5005713C,
            0xEDB88320, 0xF00F9344, 0xD6D6A3E8, 0xCB61B38C, 0x9B64C2B0, 0x86D3D2D4, 0xA00AE278, 0xBDBDF21C
        };

        constexpr uint32_t crc32(const char* s, uint32_t h = ~0)
        {
        #define CRC4(c, h) (CRC32_TABLE[((h) & 0xF) ^ (c)] ^ ((h) >> 4))
            return !*s ? ~h : crc32(s + 1, CRC4((uint8_t)*s >> 4, CRC4((uint8_t)*s & 0xF, h)));
        #undef CRC4
        }
    }

    constexpr uint32_t crc32(const char* ptr)
    {
        return detail::crc32::crc32(ptr);
    }
}

constexpr uint32_t operator "" _crc32(const char* ptr, const size_t len)
{
    return ::pride::ct::hash::detail::crc32::crc32(ptr);
}
