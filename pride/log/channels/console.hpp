
#pragma once

#include "../internal/null.hpp"
#include "../channel.hpp"
#include "../message.hpp"
#include "../fmt.hpp"
#include <memory>
#include <mutex>

namespace pride::log::channels
{
    namespace internal
    {
        // used to seperate FILE* stdout and stdout : public channel_t
        struct stdout_stream
        {
            static FILE* stream()
            {
                return stdout;
            }
        };

        struct stderr_stream
        {
            static FILE* stream()
            {
                return stderr;
            }
        };

        template<typename Mutex>
        class stdout_console : public channel_t
        {
        public:
            stdout_console()
            : channel_t()
            , _file(stdout_stream::stream())
            {
            }

            ~stdout_console() = default;
            stdout_console(const stdout_console&) = delete;
            stdout_console& operator=(const stdout_console&) = delete;

            void log(const message_t& msg) override
            {
                fmt::memory_buffer format;
                _formatter->format(msg, format);
                std::lock_guard<Mutex> lock(_mutex);
                fwrite(format.data(), sizeof(char), format.size(), _file);
                fflush(stdout_stream::stream());
            }

            void flush() override
            {
                std::lock_guard<Mutex> lock(_mutex);
                fflush(stdout_stream::stream());
            }

        private:
            Mutex _mutex;
            FILE* _file;
        };

        template<typename Mutex>
        class stderr_console : public channel_t
        {
        public:
            stderr_console()
            : _file(stderr_stream::stream())
            {
            }

            ~stderr_console() = default;
            stderr_console(const stderr_console&) = delete;
            stderr_console& operator=(const stderr_console&) = delete;

            void log(const message_t& msg) override
            {
                fmt::memory_buffer format;
                _formatter->format(msg, format);
                std::lock_guard<Mutex> lock(_mutex);
                fwrite(format.data(), sizeof(char), format.size(), _file);
                fflush(stdout_stream::stream());
            }

            void flush() override
            {
                std::lock_guard<Mutex> lock(_mutex);
                fflush(stdout_stream::stream());
            }

        private:
            Mutex _mutex;
            FILE* _file;
        };
    } // namespace internal

    using stdout_mt = internal::stdout_console<std::mutex>;
    using stdout_st = internal::stdout_console<log::internal::null_mutex>;

    using stderr_mt = internal::stderr_console<std::mutex>;
    using stderr_st = internal::stderr_console<log::internal::null_mutex>;
}
