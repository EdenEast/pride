
#pragma once

#include "../channel.hpp"
#include "../internal/null.hpp"
#include "../../os/file.hpp"
#include <mutex>

namespace pride::log::channels
{
    namespace internal
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

    using basic_file_mt = internal::basic_file<std::mutex>;
    using basic_file_st = internal::basic_file<log::internal::null_mutex>;
}
