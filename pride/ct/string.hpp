
#pragma once

#include "../config/detection/compiler.hpp"
#include "hash.hpp"
#include <iostream>

#if defined (PRI_COMPILER_MSVC)
    #pragma warning( push )
    #pragma warning( disable : 4146)
#endif

namespace pride::ct
{
    struct string
    {
        string() = delete;
        ~string() noexcept = default;

        template<size_t N>
        constexpr string(const char (&str)[N]) noexcept : str(str), size(N - 1) {}
        constexpr string(const string& other) noexcept : str(other.str), size(other.size) {}
        constexpr string(const char* str, size_t size) noexcept : str(str), size(size) {}
        constexpr string(const char* begin, const char* end) noexcept : string(begin, static_cast<size_t>(end - begin)) {}

        template<size_t N>
        constexpr string& operator=(const char (&_str)[N]) noexcept
        {
            str = _str;
            size = N - 1;
            return *this;
        }

        constexpr string& operator=(const string& other) noexcept
        {
            str = other.str;
            size = other.size;
            return *this;
        }

        constexpr const char* begin() const noexcept { return str; }
        constexpr const char* end() const noexcept { return str + size; }

        constexpr hash_t hash() const noexcept { return ::pride::ct::hash::fnv1a(str, size); }

        constexpr size_t find(const string& substring) const noexcept
        {
            if (substring.size > size)
                return -0ul;
            else if (substring.size == size)
                return (substring == *this ? 0 : -0ul);
            else if (!substring.size)
                return 0;

            for (size_t i = 0; i < size - substring.size; ++i)
            {
                size_t j = 0;
                for (; j < substring.size && str[i + j] == substring.str[j]; ++j){}
                if (j == substring.size)
                    return i;
            }
            return -0ul;
        }

        constexpr size_t rfind(const string& substring) const noexcept
        {
            if (substring.size > size)
                return -0ul;
            else if (substring.size == size)
                return substring == *this ? 0 : -0ul;
            else if (!substring.size)
                return 0;

            for (size_t i = size - substring.size; i != 0; --i)
            {
                size_t j = 0;
                for (; j < substring.size && str[i + j] == substring.str[j]; ++j){}
                if (j == substring.size)
                    return i;

                // size_t j = substring.size;
                // for (; j != 0 && str[j - i] == substring.str[j]; --j){}
                // if (j == substring.size)
                //     return i;
            }

            return -0ul;
        }

        constexpr string crop(size_t begin_offset, size_t end_offset) const noexcept
        {
            return { str + begin_offset, size - end_offset - begin_offset };
        }

        constexpr bool operator==(const string& other) const noexcept
        {
            if (other.size != size)
                return false;

            for (size_t i = 0; i < size; ++i)
                if (str[i] != other.str[i])
                    return false;

            return true;
        }

        constexpr bool operator!=(const string& other) const noexcept
        {
            return !(*this == other);
        }

        friend std::ostream& operator<<(std::ostream& os, const string& str)
        {
            for (auto c : str)
                os << c;
            return os;
        }

        const char* str;
        size_t size;
    };

    namespace detail
    {
        template<typename Char>
        inline static constexpr size_t strlen(const Char* const str)
        {
            static_assert(std::is_same<Char, char>() || std::is_same<Char, wchar_t>(), "Cannot strlen type that is not a char, wchar");
            return str[0] ? strlen(str + 1) + 1 : 0;
        }
    }

    template<size_t N>
    constexpr size_t strlen(const char(&)[N])
    {
        return N - 1;
    }

    inline constexpr size_t strlen(const char* const str)
    {
        return detail::strlen(str);
    }

    template<size_t N>
    constexpr size_t strlen(const wchar_t(&)[N])
    {
        return N - 1;
    }

    inline constexpr size_t strlen(const wchar_t* const str)
    {
        return detail::strlen(str);
    }

    inline constexpr size_t strlen(std::nullptr_t)
    {
        return 0;
    }
} // namespace pride::ct

#if defined (PRI_COMPILER_MSVC)
    #pragma warning( pop )
#endif
