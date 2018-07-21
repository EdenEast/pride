
#pragma once

#include "../channel.hpp"
#include "../../os/file.hpp"

namespace pride::log::channels
{
    template<typename Mutex>
    class basic_file : public base_channel<Mutex>
    {
    public:
        explicit basic_file(const std::string& filename, bool truncate = false)
        {
            _helper.open(filename, truncate);
        }

    protected:
        void _process(const message_t& msg, const fmt::memory_buffer& formatted) override
        {
            _helper.write(msg.raw);
        }

        void _flush() override
        {
            _helper.flush();
        }


    private:
        os::file_t _helper;
    };

    using basic_file_mt = basic_file<std::mutex>;
    // using basic_file_channel_st = basic_file<>;
}
