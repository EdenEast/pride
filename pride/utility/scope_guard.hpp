
#pragma once

#include "../preprocessor/utils.hpp"

namespace pride
{
template<typename Func>
struct scope_guard
{
    constexpr scope_guard(const Func& func)
        : func(func)
    {}
    ~scope_guard() { func(); }
    Func func;
};

namespace internal
{
    struct scope_guard_marker
    {};
    template<typename Func>
    scope_guard<Func> operator+(scope_guard_marker, Func func)
    {
        return scope_guard<Func>(func);
    }
} // namespace internal
} // namespace pride

#define defer auto PRIDE_PP_ANOYN_VARIABLE(IMPL_DEFER_) = ::pride::internal::scope_guard_marker() + [&]()
