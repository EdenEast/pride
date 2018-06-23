#pragma once

#include <cstdint>

namespace pride::hash
{
    constexpr uint32_t djb2a(const char* s, uint32_t h = 5381)
    {
        return !*s ? h : djb2a(s + 1, 33 * h ^ (uint8_t)*s);
    }
}

constexpr uint32_t operator "" _djb2a(const char* ptr, const size_t len)
{
    return ::pride::hash::djb2a(ptr);
}
