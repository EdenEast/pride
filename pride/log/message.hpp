
#pragma once

#include "sevarity.hpp"
#include "internal/thread.hpp"
#include "internal/time.hpp"
#include "fmt.hpp"
#include <chrono>
#include <string>

namespace pride::log
{
    struct message_t
    {
        message_t() = default;
        message_t(const std::string* logger_names, sevarity_t level);

        message_t(const message_t&) = delete;
        message_t& operator=(const message_t&) = delete;

        const std::string* names{nullptr};
        sevarity_t sevarity;
        std::chrono::system_clock::time_point time;
        size_t thread_id;
        fmt::memory_buffer raw;
    };

    inline message_t::message_t(const std::string* names, sevarity_t level)
    : names(names)
    , sevarity(level)
    {
        time = internal::now();
        thread_id = internal::thread_id();
    }
}
