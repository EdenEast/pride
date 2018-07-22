
#pragma once

#include "../config/detection/debug.hpp"
#include "../preprocessor/function.hpp"
#include "../utility/likely.hpp"
#include "assert.hpp"

#define PRI_IMPL_ASSERT(fail_func, expr) \
    do \
    { \
        ::pride::assert::internal::bool_expression_t eval_expr(::pride::assert::internal::expression_decomposer_t() << expr); \
        if (pride::unlikely(!eval_expr)) \
        { \
            fail_func(__FILE__, __LINE__, PRI_FUNCTION, #expr, std::move(eval_expr)); \
        } \
    } while(false)

#if defined(PRI_DEBUG)
#define PRI_ASSERT(expr) PRI_IMPL_ASSERT(::pride::assert::assert_failed_invariant, expr)
#else
#define PRI_ASSERT(expr)
#endif
