
#pragma once

#include "../channel.hpp"
#include "../detail/null.hpp"

#include <mutex>
#include <ostream>

namespace pride::log::channels
{
    namespace detail
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
    }

    using ostream_mt = detail::ostream<std::mutex>;
    using ostream_st = detail::ostream<log::detail::null_mutex>;
}
