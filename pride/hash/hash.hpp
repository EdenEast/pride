
#pragma once

#include "../config/detection/arch.hpp"
#include <cstdint>

namespace pride
{
    using hash32_t = uint32_t;
    using hash64_t = uint64_t;

#if defined(PRIDE_ARCH_64)
    using hash_t = uint64_t;
#else
    using hash_t = uint32_t;
#endif
}
