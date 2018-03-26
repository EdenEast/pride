
#pragma once

#include "expression.hpp"

namespace pride { namespace assert { namespace impl
{
    struct violation_info_t
    {
        violation_info_t() noexcept;
        violation_info_t(violation_info_t&& info) noexcept;
        violation_info_t(
              const char* file
            , int line
            , const char* function
            , const char* expr_str
            , impl::bool_expression_t expr
        ) noexcept;

        int line_number;
        const char* file_name;
        const char* function_name;
        const char* commit;
        impl::bool_expression_t expression;
    };

    inline violation_info_t::violation_info_t() noexcept
    : line_number(-1)
    , file_name(nullptr)
    , function_name(nullptr)
    , commit(nullptr)
    , expression(true)
    {
    }

    inline violation_info_t::violation_info_t(violation_info_t&& info) noexcept
    : line_number(info.line_number)
    , file_name(info.file_name)
    , function_name(info.function_name)
    , commit(info.commit)
    , expression(std::move(info.expression))
    {
    }

    inline violation_info_t::violation_info_t(
        const char* file,
        int line,
        const char* function,
        const char* expr_str,
        impl::bool_expression_t expr
    ) noexcept
    : line_number(line)
    , file_name(file)
    , function_name(function)
    , commit(expr_str)
    , expression(std::move(expr))
    {
    }
}}}
