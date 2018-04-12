
#pragma once

#include "details/algorithm.hpp"
#include <cstring>
#include <string>
#include <ostream>

namespace pride { namespace ct
{
    class cstring_t
    {
    public:
        template<std::size_t N>
        constexpr cstring_t(const char (&str)[N])
        : cstring_t{&str[0], N - 1}
        {}

        constexpr cstring_t(const char* begin, std::size_t length)
        : _str{ begin }
        , _length{ length }
        {}

        constexpr cstring_t(const char* begin, const char* end)
        : cstring_t{ begin, static_cast<std::size_t>(end - begin) }
        {}

        constexpr cstring_t(const char* begin)
        : cstring_t{ begin, length(begin) }
        {}

        static constexpr std::size_t length(const char* str)
        {
            return *str ? 1 + length(str + 1) : 0;
        }

        constexpr std::size_t length() const
        {
            return _length;
        }

        constexpr std::size_t size() const
        {
            return length();
        }

        std::string str() const
        {
            return { begin(), end() };
        }

        constexpr const char* begin() const
        {
            return _str;
        }

        constexpr const char* end() const
        {
            return _str + _length;
        }

        constexpr cstring_t operator()(std::size_t begin, std::size_t end) const
        {
            return { _str + begin, _str + end };
        }

        constexpr cstring_t pad(std::size_t begin_offset, std::size_t end_offset) const
        {
            return operator()(begin_offset, size() - end_offset);
        }

        friend std::ostream& operator<<(std::ostream& os, const cstring_t& str)
        {
            for (const char c : str)
                os << c;
            return os;
        }

    private:
        const char* _str;
        std::size_t _length;
    };

    constexpr bool operator==(const cstring_t& lhs, const cstring_t& rhs)
    {
        return pride::ct::detail::equal_range(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    constexpr bool operator!=(const cstring_t& lhs, const cstring_t& rhs)
    {
        return !(lhs == rhs);
    }

#if defined(PRI_USE_CT_TEST)

static_assert(cstring_t("one") == cstring_t("one"));
static_assert(cstring_t("one") != cstring_t("two"));

static_assert(cstring_t("size is 10").size() == 10);
static_assert(cstring_t("length is 12").length() == 12);
static_assert(cstring_t::length("length is 12") == 12);

static_assert(cstring_t("this is the beginning").begin() == cstring_t("this is the beginning"));
static_assert(cstring_t("this is the end of the line").end() == cstring_t(""));

#endif

}}
