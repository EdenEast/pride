
#pragma once

#include "../utility/shareable.hpp"
#include "fmt.hpp"
#include "formatter.hpp"
#include "message.hpp"
#include "sevarity.hpp"
#include <initializer_list>
#include <mutex>
#include <memory>
#include <string>

namespace pride::log
{
    class channel_t : public shareable<channel_t>
    {
    public:
        // using ptr = std::shared_ptr<channel_t>;
        using init_list = std::initializer_list<channel_t>;

        channel_t()
        : _formatter(std::unique_ptr<formatter_t>(new pattern_formatter_t("%+")))
        {}
        virtual ~channel_t() = default;

        virtual void log(const message_t& msg) = 0;
        virtual void flush() = 0;

        bool should_log(sevarity_t level) const;

        bool use_sevarity() const;
        channel_t& use_sevarity(bool value);

        channel_t& sevarity(sevarity_t sevarity) { _sevarity = sevarity; return *this; }
        sevarity_t sevarity() const { return _sevarity; }

        channel_t& formatter(std::unique_ptr<formatter_t> format) { _formatter = std::move(format); return *this; }
        channel_t& pattern(const std::string& pattern) { _formatter = std::unique_ptr<formatter_t>(new pattern_formatter_t(pattern)); return *this; }

    protected:
        sevarity_t _sevarity{sevarity_t::trace};
        bool _use_sevarity{false};
        std::unique_ptr<formatter_t> _formatter;
    };

    template<typename Mutex>
    class base_channel : public channel_t
    {
    public:
        base_channel() : channel_t() {}

        base_channel(const base_channel&) = delete;
        base_channel& operator=(const base_channel&) = delete;

        void log(const message_t& msg) final override;
        void flush() final override;

    protected:
        virtual void _process(const std::string& msg, const std::string& formatted) = 0;
        virtual void _flush() = 0;

        Mutex _mutex;
    };

    //
    // ─── IMPLEMENTATION ─────────────────────────────────────────────────────────────
    //

    bool channel_t::use_sevarity() const
    {
        return _use_sevarity;
    }

    channel_t& channel_t::use_sevarity(bool value)
    {
        _use_sevarity = value;
        return *this;
    }

    bool channel_t::should_log(sevarity_t level) const
    {
        return level >= _sevarity;
    }

    // ────────────────────────────────────────────────────────────────────────────────

    template<typename Mutex>
    void base_channel<Mutex>::log(const message_t& msg)
    {
        fmt::memory_buffer formatted;
        std::lock_guard<Mutex> lock(_mutex);
        _process(msg), msg;
    }

    template<typename Mutex>
    void base_channel<Mutex>::flush()
    {
        std::lock_guard<Mutex> lock(_mutex);
        _flush();
    }
}
