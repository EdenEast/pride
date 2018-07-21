
#pragma once

#include "fmt/fmt.hpp"

namespace pride::log::fmt::helper
{
    template<size_t Size>
    void append_str(const std::string& str, basic_memory_buffer<char, Size>& dest)
    {
        auto data = str.data();
        dest.append(data, data + str.size());
    }

    template<size_t Size>
    void append_str(const char* str, basic_memory_buffer<char, Size>& dest)
    {
        char data;
        while ((data = *str) != '\0')
        {
            dest.push_back(data);
            ++str;
        }
    }

    template<size_t Size1, size_t Size2>
    void append_buffer(const basic_memory_buffer<char, Size1>& buf, basic_memory_buffer<char, Size2>& dest)
    {
        auto ptr = buf.data();
        dest.append(ptr, ptr + buf.size());
    }

    template<typename T, size_t Size>
    void append_int(T n, basic_memory_buffer<char, Size>& dest)
    {
        format_int i(n);
        dest.append(i.data(), i.data() + i.size());
    }

    template<size_t Size>
    inline void pad2(int n, fmt::basic_memory_buffer<char, Size> &dest)
    {
        if (n > 99)
        {
            append_int(n, dest);
            return;
        }
        if (n > 9) // 10-99
        {
            dest.push_back('0' + (n / 10));
            dest.push_back('0' + (n % 10));
            return;
        }
        if (n >= 0) // 0-9
        {
            dest.push_back('0');
            dest.push_back('0' + n);
            return;
        }
        // negatives (unlikely, but just in case, let fmt deal with it)
        fmt::format_to(dest, "{:02}", n);
    }

    template<size_t Buffer_Size>
    void pad3(int n, fmt::basic_memory_buffer<char, Buffer_Size> &dest)
    {
        if (n > 99)
        {
            append_int(n, dest);
            return;
        }
        if (n > 9) // 10-99
        {
            dest.push_back('0');
            dest.push_back('0' + n / 10);
            dest.push_back('0' + n % 10);
            return;
        }
        if (n >= 0)
        {
            dest.push_back('0');
            dest.push_back('0');
            dest.push_back('0' + n);
            return;
        }
        // negatives (unlikely, but just in case let fmt deal with it)
        fmt::format_to(dest, "{:03}", n);
    }

    template<size_t Buffer_Size>
    void pad6(size_t n, fmt::basic_memory_buffer<char, Buffer_Size> &dest)
    {
        // todo: maybe replace this implementation with
        // pad3(n / 1000, dest);
        // pad3(n % 1000, dest);

        if (n > 99999)
        {
            append_int(n, dest);
            return;
        }
        if (n > 9999)
        {
            dest.push_back('0');
        }
        else if (n > 999)
        {
            dest.push_back('0');
            dest.push_back('0');
        }
        else if (n > 99)
        {
            dest.push_back('0');
            dest.push_back('0');
            dest.push_back('0');
        }
        else if (n > 9)
        {
            dest.push_back('0');
            dest.push_back('0');
            dest.push_back('0');
            dest.push_back('0');
        }
        else // 0-9
        {
            dest.push_back('0');
            dest.push_back('0');
            dest.push_back('0');
            dest.push_back('0');
            dest.push_back('0');
        }
        append_int(n, dest);
    }
}
