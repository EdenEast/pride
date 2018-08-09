
#pragma once

#include "../../config/detection/os.hpp"
#include "../../config/include/windows.hpp"
#include "../channel.hpp"
#include "../internal/null.hpp"
#include <mutex>

namespace pride::log::channels
{
namespace internal
{
    template<typename Mutex>
    class msvc : base_channel<Mutex>
    {
    public:
        explicit msvc()
            : base_channel<Mutex>()
        {}

    protected:
        void _process(const message_t& msg, const fmt::memory_buffer& formatted) override
        {
#if defined(PRIDE_OS_WINDOWS)
            OutputDebugStringA(fmt::to_string(formatted).c_str());
#endif
        }

        void _flush() override
        {
        }
    };
} // namespace internal

using msvc_mt = internal::msvc<std::mutex>;
using msvc_st = internal::msvc<log::internal::null_mutex>;
} // namespace pride::log::channels
