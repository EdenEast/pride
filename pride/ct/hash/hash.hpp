
#pragma once

#include "../../config/detection.hpp"
#include <cstdint>

namespace pride
{
    using hash32_t = uint32_t;
    using hash64_t = uint64_t;

#if defined(PRI_ARCH_64)
    using hash_t = uint64_t;
#else
    using hash_t = uint32_t;
#endif
    static_assert(sizeof(hash_t) == sizeof(uintptr_t), "hash_t is not the same size as the uint ptr");
}
// https://softwareengineering.stackexchange.com/a/145633
