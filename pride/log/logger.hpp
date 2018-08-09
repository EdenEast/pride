
#pragma once

#include "../utility/shareable.hpp"
#include "channel.hpp"
#include "fmt.hpp"
#include "message.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace pride::log
{
class logger_t : public shareable<logger_t>
{
public:
    // using ptr = std::shared_ptr<logger_t>;
    virtual ~logger_t() = default;

    // ─────────────────────────────────────────────────────────────────

    static logger_t::ptr create(std::string name);

    // ─────────────────────────────────────────────────────────────────

    template<typename... Args>
    void log(sevarity_t level, const char* msg, const Args&... args);

    void log(sevarity_t level, const char* msg);

    template<typename... Args>
    void trace(const char* fmt, const Args&... args);

    template<typename... Args>
    void debug(const char* fmt, const Args&... args);

    template<typename... Args>
    void info(const char* fmt, const Args&... args);

    template<typename... Args>
    void warn(const char* fmt, const Args&... args);

    template<typename... Args>
    void error(const char* fmt, const Args&... args);

    template<typename... Args>
    void crit(const char* fmt, const Args&... args);

    // ─────────────────────────────────────────────────────────────────

    template<typename T>
    void log(sevarity_t level, const T&);

    template<typename T>
    void trace(const T& msg);

    template<typename T>
    void debug(const T& msg);

    template<typename T>
    void info(const T& msg);

    template<typename T>
    void warn(const T& msg);

    template<typename T>
    void error(const T& msg);

    template<typename T>
    void crit(const T& msg);

    // ─────────────────────────────────────────────────────────────────

    void flush();
    bool should_log(sevarity_t level) { return level >= _sevarity; }

    template<typename Formatter, typename... Args>
    logger_t& set_formatter(const Args&&... args);
    logger_t& set_pattern(const std::string& pattern);

    // ─────────────────────────────────────────────────────────────────

    template<typename Channel, typename... Args>
    logger_t& add_channel(Args&&... args);
    logger_t& add_channel(const channel_t::ptr& channel);

    logger_t& name(std::string name)
    {
        _name = std::move(name);
        return *this;
    }
    logger_t& sevarity(sevarity_t level)
    {
        _sevarity = level;
        return *this;
    }
    logger_t& flush_on(sevarity_t level)
    {
        _flush_on = level;
        return *this;
    }

    // ─────────────────────────────────────────────────────────────────

    const std::string& name() { return _name; }
    const std::vector<channel_t::ptr>& channels() { return _channels; }
    sevarity_t sevarity() { return _sevarity; }
    sevarity_t flush_on() { return _flush_on; }

    // private:
    logger_t(std::string name);
    logger_t(const logger_t&) = delete;
    logger_t& operator=(const logger_t&) = delete;

private:
    virtual void _proecss(const message_t& msg);
    virtual void _flush();

    inline static std::unordered_map<std::string, logger_t::ptr> _loggers;
    std::string _name;
    std::vector<channel_t::ptr> _channels;
    sevarity_t _sevarity{ sevarity_t::info };
    sevarity_t _flush_on{ sevarity_t::off };
};

// ────────────────────────────────────────────────────────────────────────────────

inline logger_t::ptr logger_t::create(std::string name)
{
    auto instance = logger_t::make_new(std::move(name));
    _loggers.insert({ name, instance });
    return instance;
}

// ────────────────────────────────────────────────────────────────────────────────

inline logger_t::logger_t(std::string name)
    : _name(std::move(name))
{
}

// ────────────────────────────────────────────────────────────────────────────────

template<typename... Args>
void logger_t::log(sevarity_t level, const char* msg, const Args&... args)
{
    if (!should_log(level))
        return;

    // try to log and catch std::exception and anything else
    message_t message(&_name, level);
    fmt::format_to(message.raw, msg, args...);
    _proecss(message);
}

inline void logger_t::log(sevarity_t level, const char* msg)
{
    if (!should_log(level))
        return;

    // try to log and catch std::exception and anything else
    message_t message(&_name, level);
    fmt::format_to(message.raw, "{}", msg);
    _proecss(message);
}

template<typename... Args>
void logger_t::trace(const char* fmt, const Args&... args)
{
    log(sevarity_t::trace, fmt, args...);
}

template<typename... Args>
void logger_t::debug(const char* fmt, const Args&... args)
{
    log(sevarity_t::debug, fmt, args...);
}

template<typename... Args>
void logger_t::info(const char* fmt, const Args&... args)
{
    log(sevarity_t::info, fmt, args...);
}

template<typename... Args>
void logger_t::warn(const char* fmt, const Args&... args)
{
    log(sevarity_t::warn, fmt, args...);
}

template<typename... Args>
void logger_t::error(const char* fmt, const Args&... args)
{
    log(sevarity_t::error, fmt, args...);
}

template<typename... Args>
void logger_t::crit(const char* fmt, const Args&... args)
{
    log(sevarity_t::critical, fmt, args...);
}

// ────────────────────────────────────────────────────────────────────────────────

template<typename T>
void logger_t::log(sevarity_t level, const T& value)
{
    if (!should_log(level))
        return;

    message_t message(&_name, level);
    fmt::format_to(message.raw, "{}", value);
    _proecss(message);
}

template<typename T>
void logger_t::trace(const T& msg)
{
    log(sevarity_t::trace, msg);
}

template<typename T>
void logger_t::debug(const T& msg)
{
    log(sevarity_t::trace, msg);
}

template<typename T>
void logger_t::info(const T& msg)
{
    log(sevarity_t::trace, msg);
}

template<typename T>
void logger_t::warn(const T& msg)
{
    log(sevarity_t::trace, msg);
}

template<typename T>
void logger_t::error(const T& msg)
{
    log(sevarity_t::trace, msg);
}

template<typename T>
void logger_t::crit(const T& msg)
{
    log(sevarity_t::critical, msg);
}

// ────────────────────────────────────────────────────────────────────────────────

template<typename Channel, typename... Args>
logger_t& logger_t::add_channel(Args&&... args)
{
    _channels.push_back(std::make_shared<Channel>(std::forward<Args>(args)...));
    return *this;
}

inline logger_t& logger_t::add_channel(const channel_t::ptr& channel)
{
    _channels.push_back(channel);
    return *this;
}

template<typename Formatter, typename... Args>
logger_t& logger_t::set_formatter(const Args&&... args)
{
    for (auto& channel : _channels)
    {
        auto fmt = std::make_unique<Formatter>(std::forward<Args>(args)...);
        channel->formatter(std::move(fmt));
    }
    return *this;
}

inline logger_t& logger_t::set_pattern(const std::string& pattern)
{
    for (auto& channel : _channels)
        channel->pattern(pattern);
    return *this;
}

// ────────────────────────────────────────────────────────────────────────────────

inline void logger_t::_proecss(const message_t& msg)
{
    for (auto& channel : _channels)
    {
        auto level = msg.sevarity;

        // check to see if the channel should use it's level.
        // if so then also check if it should log.
        // or if the logger's level should log.
        // then we can log it to the channel
        if ((channel->use_sevarity() && channel->should_log(level)) || should_log(level))
            channel->log(msg);
    }
}

inline void logger_t::_flush()
{
    for (auto& channel : _channels)
        channel->flush();
}

// ────────────────────────────────────────────────────────────────────────────────
} // namespace pride::log
