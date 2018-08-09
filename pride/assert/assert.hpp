
#pragma once

#include "violation.hpp"
#include <functional>
#include <stdexcept>
#include <sstream>

namespace pride::assert
{
    inline std::ostream& assert_fail_default_log(std::ostream& os, const internal::violation_info_t& info) noexcept
    {
        using namespace console;
        const std::ios_base::fmtflags flags = os.flags();

        os  << std::boolalpha << style_t::bold
            << fgb_t::grey    << info.file_name << ':' << info.line_number << ':' << info.function_name << ": "
            << fgb_t::black   << style_t::underline << "Assertion"
            << fg_t::reset    << style_t::reset << " `"
                              << internal::expression_output_t(info.commit, info.expression)
                              << "`, with expression `" << info.expression << "` "
            << fg_t::red      << style_t::underline << style_t::bold << "failed"
            << fg_t::reset    << style_t::reset << ".\n";

        os.flags(flags);
        return os;
    }

    inline void assert_fail_default_log(const internal::violation_info_t& info) noexcept
    {
        // Must flush because we will abort()
        assert_fail_default_log(std::cerr, info).flush();
    }

    inline std::string assert_fail_to_string(const internal::violation_info_t& info)
    {
        std::ostringstream os;
        assert_fail_default_log(os, info);
        return os.str();
    }

    class assert_message_t : public std::invalid_argument
    {
    public:
        assert_message_t(internal::violation_info_t& info);
        friend std::ostream& operator<<(std::ostream& os, assert_message_t& msg);
        internal::violation_info_t info;
    };

    inline assert_message_t::assert_message_t(internal::violation_info_t& info)
    : std::invalid_argument(assert_fail_to_string(info))
    , info(std::move(info))
    {
    }

    inline std::ostream& operator<<(std::ostream& os, const assert_message_t& error)
    {
        return assert_fail_default_log(os, error.info);
    }

    inline void assert_failed_default(internal::violation_info_t info) noexcept
    {
        using namespace std;

        {
            // abort prevents destructors from running so this will make sure that
            // info is destoryed when leaving this scope. This prevents leaks.
            internal::violation_info_t _info(std::move(info));
            assert_fail_default_log(_info);
        }
        abort();
    }

    inline void assert_failed_invariant(
        const char* file,
        int line,
        const char* function,
        const char* expr_str,
        internal::bool_expression_t expr
    ) noexcept
    {
        assert_failed_default(internal::violation_info_t(file, line, function, expr_str, std::move(expr)));
    }
} // namespace pride::assert
