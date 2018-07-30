
#pragma once

#include "../../config/detection/os.hpp"
#include "../channel.hpp"
#include "../fmt.hpp"
#include <array>
#include <string>

#if !defined(PRIDE_OS_WINDOWS)
#include <syslog.h>
#endif

namespace pride::log::channels
{
#if !defined(PRIDE_OS_WINDOWS)
    class syslog_t : public channel_t
    {
    public:
        syslog_t(const std::string& indent = "", int option = 0, int facility = LOG_USER)
        {
            _properties[static_cast<size_t>(sevarity_t::trace)] = LOG_DEBUG;
            _properties[static_cast<size_t>(sevarity_t::info)] = LOG_INFO;
            _properties[static_cast<size_t>(sevarity_t::debug)] = LOG_DEBUG;
            _properties[static_cast<size_t>(sevarity_t::warn)] = LOG_WARNING;
            _properties[static_cast<size_t>(sevarity_t::error)] = LOG_ERR;
            _properties[static_cast<size_t>(sevarity_t::critical)] = LOG_CRIT;
            _properties[static_cast<size_t>(sevarity_t::off)] = LOG_INFO;

            ::openlog(_indent.empty() ? nullptr : _indent.c_str(), option, facility);
        }

        ~syslog_t()
        {
            ::closelog();
        }

        syslog_t(const syslog_t&) = delete;
        syslog_t& operator=(const syslog_t&) = delete;

        void log(const message_t& msg) override
        {
            ::syslog(prop_from_sevarity(msg), "%s", fmt::to_string(msg.raw).c_str());
        }

        void flush() override {}

    private:
        int prop_from_sevarity(const message_t& msg) const
        {
            return _properties[static_cast<size_t>(msg.sevarity)];
        }

        std::array<int, 7> _properties;
        const std::string _indent;
    };
#else
    class syslog_t : public channel_t
    {
    public:
        syslog_t(const std::string& indent = "", int option = 0, int facility = 0){}
        ~syslog_t() {}

        syslog_t(const syslog_t&) = delete;
        syslog_t& operator=(const syslog_t&) = delete;

        void log(const message_t& msg) override {}
        void flush() override {}

    };
#endif
}
