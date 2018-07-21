
#pragma once

#include "../../config/detection/os.hpp"
#include "../../config/include/windows.hpp"
#include "../detail/null.hpp"
#include "../channel.hpp"
#include <mutex>

namespace pride::log::channels
{
    namespace detail
    {
        template<typename Mutex>
        class msvc : base_channel<Mutex>
        {
        public:
            explicit msvc() : base_channel<Mutex>() {}

        protected:
            void _process(const message_t& msg, const fmt::memory_buffer& formatted) override
            {
#if defined(PRI_OS_WINDOWS)
                OutputDebugStringA(fmt::to_string(formatted).c_str());
#endif
            }

            void _flush() override
            {
            }
        };
    }

    using msvc_mt = detail::msvc<std::mutex>;
    using msvc_st = detail::msvc<log::detail::null_mutex>;
}
