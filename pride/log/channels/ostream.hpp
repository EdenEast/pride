
#pragma once

#include "../channel.hpp"
#include "../internal/null.hpp"

#include <mutex>
#include <ostream>

namespace pride::log::channels
{
namespace internal
{
    template<typename Mutex>
    class ostream : public base_channel<Mutex>
    {
    public:
        explicit ostream(std::ostream& os, bool force_flush = false)
            : _os(os)
            , _force_flush(force_flush)
        {
        }

        ostream(const ostream&) = delete;
        ostream& operator=(const ostream&) = delete;

    protected:
        void _process(const message_t& msg, const fmt::memory_buffer& formatted) override
        {
            _os.write(formatted.data(), formatted.size());
            if (_force_flush)
                _os.flush();
        }

        void _flush() override
        {
            _os.flush();
        }

    private:
        std::ostream _os;
        bool _force_flush;
    };
} // namespace internal

using ostream_mt = internal::ostream<std::mutex>;
using ostream_st = internal::ostream<log::internal::null_mutex>;
} // namespace pride::log::channels
