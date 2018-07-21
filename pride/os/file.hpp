
#pragma once

#include "../config/detection/os.hpp"
#include "../config/detection/arch.hpp"
#include "../config/include/windows.hpp"
#include "../log/fmt.hpp"
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include <thread>
#include <tuple>
#include <experimental/filesystem>

#if !defined(PRI_EOL)
#if defined(PRI_OS_WINDOWS)
#define PRI_EOL "\r\n"
#else
#define PRI_EOL "\n"
#endif
#endif

namespace pride::os
{
    // namespace file
    // {
    //     size_t filesize(FILE* f)
    //     {
    //         if (f == nullptr)
    //         {
    //             // failed
    //         }

    //     #if defined(PRI_OS_WINDOWS)
    //         int fd = _fileno(f);

    //     #if defined(PRI_ARCH_64)
    //         struct _stat64 st
    //         if (_fstat64(fd, &st) == 0)
    //             return st.st_size;
    //     #else
    //         long ret = _filelength(fd);
    //         if (ret >= 0)
    //             return static_cast<size_t>(ret);
    //     #endif
    //     #else
    //     #if defined
    //     #endif
    //     }
    // }

    class file_t
    {
    public:
        static constexpr const char* default_eol = PRI_EOL;
        explicit file_t() = default;
        file_t(const file_t&) = delete;
        file_t& operator=(const file_t&) = delete;

        ~file_t()
        {
            close();
        }

        void open(std::string name, bool truncate = false)
        {
            close();

            auto mode = truncate ? "wb" : "ab";
            _name = std::move(name);

#if defined (PRI_OS_WINDOWS)
            fopen_s(&_file, _name.c_str(), mode);
#else
            _file = std::fopen(name.c_str(), mode);
#endif
        }

        void reopen(bool truncate)
        {
            // if (_name.empty())
            // {
            //     throw std::exception("Failed to reopen file - file was not openned before");
            // }

            open(_name, truncate);
        }

        void flush()
        {
            std::fflush(_file);
        }

        void close()
        {
            if (_file != nullptr)
            {
                std::fclose(_file);
                _file = nullptr;
            }
        }

        void write(const std::string& buffer)
        {
            auto size = buffer.size();
            auto data = buffer.data();
            if (std::fwrite(data, 1, size, _file) != size)
            {
                // throw error("Failed to write to file: " + _name);
            }
        }

        void write(const log::fmt::memory_buffer& buffer)
        {
            auto size = buffer.size();
            auto data = buffer.data();
            if (std::fwrite(data, 1, size, _file) != size)
            {
                // throw error("Failed to write to file: " + _name);
            }
        }

        // size_t size() const
        // {
        // }

        const std::string& name() const
        {
            return _name;
        }

    private:
        FILE* _file = nullptr;
        std::string _name;
    };
}
