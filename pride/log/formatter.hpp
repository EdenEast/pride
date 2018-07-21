
#pragma once

#include "fmt.hpp"
#include "message.hpp"
#include <string>

namespace pride::log
{
    class formatter_t
    {
    public:
        virtual ~formatter_t() = default;
        virtual void format(const message_t& msg, fmt::memory_buffer& buffer) = 0;
    };

    class pattern_formatter_t : public formatter_t
    {
    public:
        pattern_formatter_t(const std::string& pattern) {}

        void format(const message_t& msg, fmt::memory_buffer& buffer) {}
    };
}
