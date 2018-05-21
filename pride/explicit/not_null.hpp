
#pragma once

#include <utility>
#include <memory>

namespace pride
{
    namespace detail
    {
        template<typename Ptr>
        struct not_null
        {
            Ptr ptr;

            explicit not_null(Ptr ptr) : ptr(::std::move(ptr)) {}
            explicit not_null(std::nullptr_t) = delete;
            Ptr& get_nested() { return ptr; }
            Ptr const& get_nested() const { return ptr; }
            auto operator*() const noexcept(noexcept(*ptr)) -> decltype(*ptr) { return *ptr; }
            auto operator*() noexcept(noexcept(*ptr)) -> decltype(*ptr) { return *ptr; }
            auto operator->() const noexcept(noexcept(std::addressof(*ptr))) -> decltype(std::addressof(*ptr)) { return std::addressof(*ptr); }
            auto operator->() noexcept(noexcept(std::addressof(*ptr))) -> decltype(std::addressof(*ptr)) { return std::addressof(*ptr); }
        };

        template<typename Ptr>
        not_null<Ptr> as_not_null(Ptr p)
        {
            return not_null<Ptr>(std::move(p));
        }

        inline void as_not_null(::std::nullptr_t) = delete;
    }

    using detail::not_null;
    using detail::as_not_null;
}
