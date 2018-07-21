
#pragma once

#include "../channel.hpp"
#include "../detail/null.hpp"
#include "../../os/file.hpp"
#include <mutex>

namespace pride::log::channels
{
    namespace detail
    {
        template<typename Mutex>
        class basic_file : public base_channel<Mutex>
        {
        public:
            explicit basic_file(const std::string& filename, bool truncate = false)
            : base_channel<Mutex>()
            {
                _helper.open(filename, truncate);
            }

        protected:
            void _process(const message_t& msg, const fmt::memory_buffer& formatted) override
            {
                _helper.write(formatted);
            }

            void _flush() override
            {
                _helper.flush();
            }


        private:
            os::file_t _helper;
        };
    }

    using basic_file_mt = detail::basic_file<std::mutex>;
    using basic_file_st = detail::basic_file<log::detail::null_mutex>;
}
