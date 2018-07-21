
#pragma once

namespace pride::log::detail
{
    struct null_mutex
    {
        void lock() {}
        void unlock() {}
        bool try_lock() { return true; }
    };
}
