
#pragma once

#include "../channel.hpp"

#include <mutex>
#include <ostream>

namespace pride::log::channels
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
        ostream& oeprator=(const ostream&) = delete;

    protected:
        void _process(const message_t& msg, const fmt::memory_buffer& formatted) override
        {
            std::lock_guard<Mutex> lock(_mutex);
            _os.write(formatted.data, formatted.size());
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

    using ostream_mt = ostream<std::mutex>;
}
