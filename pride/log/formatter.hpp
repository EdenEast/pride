
#pragma once

#include "fmt.hpp"
#include "message.hpp"
#include "detail/thread.hpp"
#include "detail/time.hpp"
#include <chrono>
#include <ctime>
#include <memory>
#include <thread>
#include <string>
#include <vector>

namespace pride::log
{
    class formatter_t
    {
    public:
        virtual ~formatter_t() = default;
        virtual void format(const message_t& msg, fmt::memory_buffer& buffer) = 0;
    };

    namespace detail
    {
        class flag_formatter_t
        {
        public:
            virtual ~flag_formatter_t() = default;
            virtual void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer) = 0;
        };

        //
        // ─── NAME AND LEVEL FORMAT ───────────────────────────────────────
        //

        class name_formatter_t : public flag_formatter_t
        {
        public:
            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                fmt::helper::append_str(*msg.names, buffer);
            }
        };

        class level_formatter_t : public flag_formatter_t
        {
        public:
            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                fmt::helper::append_str(detail::to_long_name(msg.sevarity), buffer);
                //fmt::helper::append_str(pride::log::detail::sevarity::to_long_name(msg.sevarity), buffer);
            }
        };

        class short_level_formatter_t : public flag_formatter_t
        {
        public:
            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                fmt::helper::append_str(detail::to_short_name(msg.sevarity), buffer);
            }
        };

        //
        // ─── DATE TIME PATTERN ───────────────────────────────────────────
        //

        static const char* ampm(const tm& t)
        {
            return t.tm_hour >= 12 ? "PM" : "AM";
        }

        static unsigned int to12h(const tm& t)
        {
            return t.tm_hour > 12 ? t.tm_hour - 12 : t.tm_hour;
        }

        class short_weekday_formatter_t : public flag_formatter_t
        {
        public:
            static constexpr std::array<const char*, 7> days {{
                "Sun","Mon","Tue","Wed","Thu","Fri","Sat"
            }};

            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                fmt::helper::append_str(days[tm_time.tm_wday], buffer);
            }
        };

        class long_weekday_formatter_t : public flag_formatter_t
        {
        public:
            static constexpr std::array<const char*, 7> days {{
                    "Sunday","Monday","Tueday","Wednesday","Thursday","Friday","Saturday"
            }};

            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                fmt::helper::append_str(days[tm_time.tm_wday], buffer);
            }
        };

        class short_month_formatter_t : public flag_formatter_t
        {
        public:
            static constexpr std::array<const char*, 12> monhths {{
                "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"
            }};

            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                fmt::helper::append_str(monhths[tm_time.tm_mon], buffer);
            }
        };

        class long_month_formatter_t : public flag_formatter_t
        {
        public:
            static constexpr std::array<const char*, 12> monhths {{
                "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
            }};

            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                fmt::helper::append_str(monhths[tm_time.tm_mon], buffer);
            }
        };

        class date_time_formatter_t : public flag_formatter_t
        {
        public:
            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                // Date
                fmt::helper::append_str(short_weekday_formatter_t::days[tm_time.tm_wday], buffer);
                buffer.push_back(' ');
                fmt::helper::append_str(short_month_formatter_t::monhths[tm_time.tm_mon], buffer);
                buffer.push_back(' ');
                fmt::helper::append_int(tm_time.tm_mday, buffer);
                buffer.push_back(' ');

                // Time
                fmt::helper::pad2(tm_time.tm_hour, buffer);
                buffer.push_back(':');
                fmt::helper::pad2(tm_time.tm_min, buffer);
                buffer.push_back(':');
                fmt::helper::pad2(tm_time.tm_sec, buffer);
                buffer.push_back(' ');
                fmt::helper::append_int(tm_time.tm_year  + 1900, buffer);
            }
        };

        class year_2_digit_formatter_t : public flag_formatter_t
        {
        public:
            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                fmt::helper::pad2(tm_time.tm_year % 100, buffer);
            }
        };

        class year_4_digit_formatter_t : public flag_formatter_t
        {
        public:
            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                fmt::helper::append_int(tm_time.tm_year + 1900, buffer);
            }
        };

        // Short MM/DD/YY date, equivalent to %m/%d/%y 08/23/01
        class short_date_formatter_t : public flag_formatter_t
        {
        public:
            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                fmt::helper::pad2(tm_time.tm_mon + 1, buffer);
                buffer.push_back('/');
                fmt::helper::pad2(tm_time.tm_mday, buffer);
                buffer.push_back('/');
                fmt::helper::pad2(tm_time.tm_year % 100, buffer);
            }
        };

        class number_month_formatter_t : public flag_formatter_t
        {
        public:
            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                fmt::helper::pad2(tm_time.tm_mon + 1, buffer);
            }
        };

        class number_day_formatter_t : public flag_formatter_t
        {
        public:
            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                fmt::helper::pad2(tm_time.tm_mday, buffer);
            }
        };

        class military_time_hours_formatter_t : public flag_formatter_t
        {
        public:
            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                fmt::helper::pad2(tm_time.tm_hour, buffer);
            }
        };

        class twelve_hour_formatter_t : public flag_formatter_t
        {
        public:
            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                fmt::helper::pad2(to12h(tm_time), buffer);
            }
        };

        class minute_formatter_t : public flag_formatter_t
        {
        public:
            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                fmt::helper::pad2(tm_time.tm_min, buffer);
            }
        };

        class second_formatter_t : public flag_formatter_t
        {
        public:
            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                fmt::helper::pad2(tm_time.tm_sec, buffer);
            }
        };

        class milisecond_formatter_t : public flag_formatter_t
        {
        public:
            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                auto duration = msg.time.time_since_epoch();
                auto milisec = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000;
                fmt::helper::pad3(static_cast<int>(milisec), buffer);
            }
        };

        class microsecond_formatter_t : public flag_formatter_t
        {
        public:
            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                auto duration = msg.time.time_since_epoch();
                auto microsec = std::chrono::duration_cast<std::chrono::microseconds>(duration).count() % 1000000;
                fmt::helper::pad3(static_cast<int>(microsec), buffer);
            }
        };

        class nanosecond_formatter_t : public flag_formatter_t
        {
        public:
            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                auto duration = msg.time.time_since_epoch();
                auto microsec = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() % 1000000000;
                fmt::helper::pad3(static_cast<int>(microsec), buffer);
            }
        };

        class seconds_since_epoch_formatter_t : public flag_formatter_t
        {
        public:
            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                auto duration = msg.time.time_since_epoch();
                auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
                fmt::helper::append_int(seconds, buffer);
            }
        };

        class ampm_formatter_t : public flag_formatter_t
        {
        public:
            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                fmt::helper::append_str(ampm(tm_time), buffer);
            }
        };

        class twenty_four_hour_time_formatter_t : public flag_formatter_t
        {
        public:
            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                fmt::helper::pad2(tm_time.tm_hour, buffer);
                buffer.push_back(':');
                fmt::helper::pad2(tm_time.tm_min, buffer);
                fmt::helper::append_str(ampm(tm_time), buffer);
            }
        };

        class twelve_hour_time_formatter_t : public flag_formatter_t
        {
        public:
            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                fmt::helper::pad2(to12h(tm_time), buffer);
                buffer.push_back(':');
                fmt::helper::pad2(tm_time.tm_min, buffer);
                buffer.push_back(':');
                fmt::helper::pad2(tm_time.tm_sec, buffer);
                buffer.push_back(' ');
                fmt::helper::append_str(ampm(tm_time), buffer);
            }
        };

        class iso_8601_time_formatter_t : public flag_formatter_t
        {
        public:
            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                fmt::helper::pad2(tm_time.tm_hour, buffer);
                buffer.push_back(':');
                fmt::helper::pad2(tm_time.tm_min, buffer);
                buffer.push_back(':');
                fmt::helper::pad2(tm_time.tm_sec, buffer);
            }
        };

        class iso_8601_time_from_utc_formatter_t : public flag_formatter_t
        {
        public:
            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                // @Todo:
            }
        };

        //
        // ─── THREAD AND PROECSS ──────────────────────────────────────────
        //

        class thread_id_formatter_t : public flag_formatter_t
        {
        public:
            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                // @Todo:
                // fmt::helper::pad6(msg.thread_id);
            }
        };

        class pid_formatter_t : public flag_formatter_t
        {
        public:
            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                // @Todo:
            }
        };

        //
        // ─── RAW MESSAGE ─────────────────────────────────────────────────
        //

        class message_formatter_t : public flag_formatter_t
        {
        public:
            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                fmt::helper::append_buffer(msg.raw, buffer);
            }
        };

        class single_char_formatter_t : public flag_formatter_t
        {
        public:
            explicit single_char_formatter_t(char ch) : _ch(ch) {}
            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                buffer.push_back(_ch);
            }
        private:
            char _ch;
        };

        class aggregate_formatter_t : public flag_formatter_t
        {
        public:
            aggregate_formatter_t() = default;

            void add_ch(char ch)
            {
                _str += ch;
            }

            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                fmt::helper::append_str(_str, buffer);
            }

        private:
            std::string _str;
        };

        class full_formatter_t : public flag_formatter_t
        {
        public:
            void format(const message_t& msg, const std::tm& tm_time, fmt::memory_buffer& buffer)
            {
                auto duration = msg.time.time_since_epoch();
                std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);

                if (_cache_timestamp != seconds || _cached_datetime.size() == 0)
                {
                    _cached_datetime.resize(0);
                    _cached_datetime.push_back('[');
                    fmt::helper::append_int(tm_time.tm_year + 1900, _cached_datetime);
                    _cached_datetime.push_back('-');

                    fmt::helper::pad2(tm_time.tm_mon + 1, _cached_datetime);
                    _cached_datetime.push_back('-');

                    fmt::helper::append_int(tm_time.tm_mday, _cached_datetime);
                    _cached_datetime.push_back(' ');

                    fmt::helper::append_int(tm_time.tm_hour, _cached_datetime);
                    _cached_datetime.push_back(':');

                    fmt::helper::append_int(tm_time.tm_min, _cached_datetime);
                    _cached_datetime.push_back(':');

                    fmt::helper::append_int(tm_time.tm_sec, _cached_datetime);
                    _cached_datetime.push_back('.');

                    _cache_timestamp = seconds;
                }
                fmt::helper::append_buffer(_cached_datetime, buffer);

                auto milisec = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000;
                if (milisec != _milisec_cache_timestamp || _cached_miliseconds.size() == 0)
                {
                    _cached_miliseconds.resize(0);
                    fmt::helper::pad3(static_cast<int>(milisec), _cached_miliseconds);
                    _cached_miliseconds.push_back(']');
                    _cached_miliseconds.push_back(' ');
                    _milisec_cache_timestamp = milisec;
                }
                fmt::helper::append_buffer(_cached_miliseconds, buffer);

                buffer.push_back('[');
                fmt::helper::append_str(detail::to_long_name(msg.sevarity), buffer);
                buffer.push_back(']');
                buffer.push_back(' ');
                fmt::helper::append_buffer(msg.raw, buffer);
            }

        private:
            std::chrono::seconds _cache_timestamp{0};
            std::chrono::milliseconds::rep _milisec_cache_timestamp{0};
            fmt::basic_memory_buffer<char, 128> _cached_datetime;
            fmt::basic_memory_buffer<char, 8> _cached_miliseconds;
        };
    } // namespace detail

    class pattern_formatter_t : public formatter_t
    {
    public:
        explicit pattern_formatter_t(const std::string& pattern)
        {
            std::memset(&_cached_tm, 0, sizeof(_cached_tm));
            compile_pattern(pattern);
        }
        pattern_formatter_t(const pattern_formatter_t&) = delete;
        pattern_formatter_t& operator=(const pattern_formatter_t&) = delete;

        void format(const message_t& msg, fmt::memory_buffer& buffer) override
        {
            auto seconds = std::chrono::duration_cast<std::chrono::seconds>(msg.time.time_since_epoch());
            if (seconds != _last_log_seconds)
            {
                _cached_tm = get_time(msg);
                _last_log_seconds = seconds;
            }

            for (auto& f : _formatters)
                f->format(msg, _cached_tm, buffer);

            fmt::helper::append_str("\n", buffer);
        }

    private:
        void handle_flag(char flag)
        {
            switch (flag)
            {
            case 'n': // logger name
                _formatters.emplace_back(new detail::name_formatter_t()); break;
            case 'l': // long level name
                _formatters.emplace_back(new detail::level_formatter_t()) ; break;
            case 'L': // short level name
                _formatters.emplace_back(new detail::short_level_formatter_t()) ; break;
            case 't': // thread id
                _formatters.emplace_back(new detail::thread_id_formatter_t()) ; break;
            case 'P': // process id
                _formatters.emplace_back(new detail::pid_formatter_t()) ; break;
            // case 'i': // message counter
            //     _formatters.emplace_back(new detail::message_counter_formatter_t()) ; break;
            case 'v': // message
                _formatters.emplace_back(new detail::message_formatter_t()) ; break;
            case '+': // message
                _formatters.emplace_back(new detail::full_formatter_t()) ; break;

            case 'a': // abrev weekday names
                _formatters.emplace_back(new detail::short_weekday_formatter_t()) ; break;
            case 'A': // full weekday names
                _formatters.emplace_back(new detail::long_weekday_formatter_t()) ; break;
            case 'b': // abrev month name
                _formatters.emplace_back(new detail::short_month_formatter_t()) ; break;
            case 'B': // full month name
                _formatters.emplace_back(new detail::short_month_formatter_t()) ; break;

            case 'c': // date and time (Mon Jun 17 12:14:32 2018)
                _formatters.emplace_back(new detail::date_time_formatter_t()) ; break;
            case 'C': // year in 2 digits (18)
                _formatters.emplace_back(new detail::year_2_digit_formatter_t()) ; break;
            case 'Y': // year 4 digits (2018)
                _formatters.emplace_back(new detail::year_4_digit_formatter_t()) ; break;
            case 'D': // short date MM/DD/YY date eq %m/%d/%y 09/14/18
                _formatters.emplace_back(new detail::short_date_formatter_t()) ; break;
            case 'm': // month in number 1-12
                _formatters.emplace_back(new detail::number_month_formatter_t()) ; break;
            case 'd': // day in number 1-31
                _formatters.emplace_back(new detail::number_day_formatter_t()) ; break;

            case 'H': // hours in 24 hour format 1-24
                _formatters.emplace_back(new detail::military_time_hours_formatter_t()) ; break;
            case 'I': // hours in a 12 hour format 1-12
                _formatters.emplace_back(new detail::twelve_hour_formatter_t()) ; break;
            case 'M': // minutes 0-59
                _formatters.emplace_back(new detail::minute_formatter_t()) ; break;
            case 'S': // seconds 0-59
                _formatters.emplace_back(new detail::second_formatter_t()) ; break;
            case 'e': // miliseconds 000 - 999
                _formatters.emplace_back(new detail::milisecond_formatter_t()) ; break;
            case 'f': // microseconds 000000 - 999999
                _formatters.emplace_back(new detail::microsecond_formatter_t()) ; break;
            case 'F': // nanoseconds 000000 - 999999
                _formatters.emplace_back(new detail::nanosecond_formatter_t()) ; break;
            case 'E': // seconds since epoch
                _formatters.emplace_back(new detail::seconds_since_epoch_formatter_t()) ; break;

            case 'p': // seconds since epoch
                _formatters.emplace_back(new detail::ampm_formatter_t()) ; break;
            case 'r': // 12 hour clock 03:24:02 am
                _formatters.emplace_back(new detail::twelve_hour_time_formatter_t()) ; break;
            case 'R': // 24 hour clock 23:53 eq %H:%M
                _formatters.emplace_back(new detail::twenty_four_hour_time_formatter_t()) ; break;
            case 'T': // iso 8601 time format (HH:MM:SS) eq %H:%M:%S
                _formatters.emplace_back(new detail::iso_8601_time_formatter_t()) ; break;
            case 'z': // iso 8601 offset from UTC in time zone
                _formatters.emplace_back(new detail::iso_8601_time_from_utc_formatter_t()) ; break;

            default:
                _formatters.emplace_back(new detail::single_char_formatter_t('%'));
                _formatters.emplace_back(new detail::single_char_formatter_t(flag));
                break;
            };
        }

        void compile_pattern(const std::string pattern)
        {
            auto end = pattern.end();
            std::unique_ptr<detail::aggregate_formatter_t> user_chars;
            for (auto it = pattern.begin(); it != end; ++it)
            {
                if (*it == '%')
                {
                    if (user_chars)
                        _formatters.push_back(std::move(user_chars));

                    if (++it != end)
                        handle_flag(*it);
                    else
                        break;
                }
                else
                {
                    if (!user_chars)
                        user_chars = std::unique_ptr<detail::aggregate_formatter_t>(new detail::aggregate_formatter_t());
                    user_chars->add_ch(*it);
                }
            }

            if (user_chars)
                _formatters.push_back(std::move(user_chars));
        }

        std::tm get_time(const message_t& msg)
        {
            return detail::local_time(std::chrono::system_clock::to_time_t(msg.time));
            //return detail::local_time(std::chrono::system_clock::to_time_t(msg.time));
        }

        std::tm _cached_tm;
        std::chrono::seconds _last_log_seconds;
        std::vector<std::unique_ptr<detail::flag_formatter_t>> _formatters;
    };
}
