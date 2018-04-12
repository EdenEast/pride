
#pragma once

#include "../config/detection/os.hpp"
#include "../utility/scope_guard.hpp"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>

#if defined(PRI_OS_MACOSX) || defined(PRI_OS_LINUX)
#include <unistd.h>
#elif defined(PRI_OS_WINDOWS)
#include <io.h>
#include "../config/include/windows.hpp"
#endif

#if defined(PRI_OS_WINDOWS)
    // Only defined in windows 10 onwards, redefining in lower windows since it
    // doesn't gets used in lower versions
    // https://docs.microsoft.com/en-us/windows/console/getconsolemode
    #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
    #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
    #endif
#endif

namespace pride { namespace console
{
    namespace detail
    {
#if defined(PRI_OS_WINDOWS)
        bool is_msys_pty(int fd) noexcept;
        HANDLE get_console_handle(const std::streambuf* stream) noexcept;
        bool set_win_term_ansi_colors(const std::streambuf* stream) noexcept;
        bool support_ansi(const std::streambuf* stream) noexcept;
#endif
    }

    // --------------------------------------------------------------------------------------------

    bool has_color_support();
    bool is_terminal(const std::streambuf* os_buffer);

    // --------------------------------------------------------------------------------------------

    inline bool has_color_support()
    {
#if defined(PRI_OS_WINDOWS)
        // All windows versions support colors though native console functions
        return true;
#else
        const char* terms[] = {
            "ansi", "color", "console", "cygwin", "gnome", "konsole",
            "kterm", "linux", "msys", "putty", "rxvt", "screen",
            "vt100", "xterm"
        };

        auto env = std::getenv("TERM");
        if (env == nullptr)
            return false;

        return std::any_of(std::begin(terms), std::end(terms),
            [&](const char* term){
                return std::strstr(env, term) != nullptr;
            }
        );
#endif
    }

    inline bool is_terminal(const std::streambuf* os_buffer)
    {
#if defined(PRI_OS_MACOSX) || defined(PRI_OS_LINUX)
        if (os_buffer == std::cout.rdbuf())
        {
            static const bool cout_term = isatty(fileno(stdout)) != 0;
            return cout_term;
        }
        else if (os_buffer == std::cerr.rdbuf())
        {
            static const bool cerr_term = isatty(fileno(stderr)) != 0;
            return cerr_term;
        }
#elif defined (PRI_OS_WINDOWS)
        if (os_buffer == std::cout.rdbuf())
        {
            static const bool cout_term =
                (_isatty(_fileno(stdout)) || detail::is_msys_pty(_fileno(stdout)));
            return cout_term;
        }
        else if (os_buffer == std::cerr.rdbuf())
        {
            static const bool cerr_term =
                (_isatty(_fileno(stderr)) || detail::is_msys_pty(_fileno(stderr)));
            return cerr_term;
        }
#endif
        return false;
    }


#if defined(PRI_OS_WINDOWS)
    namespace detail
    {
        inline bool is_msys_pty(int fd) noexcept
        {
            // Dynamic load for binary compability with old Windows
            const auto file_information_handler
                = reinterpret_cast<decltype(&GetFileInformationByHandleEx)>(
                    GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetFileInformationByHandleEx"));

            if (!file_information_handler)
                return false;

            HANDLE h = reinterpret_cast<HANDLE>(_get_osfhandle(fd));
            if (h == INVALID_HANDLE_VALUE)
                return false;

            // Checking that it is not a pipe
            if (GetFileType(h) != FILE_TYPE_PIPE)
                return false;

            // POD type for FILE_NAME_INFO from WinBase.h
            struct file_name_info_t
            {
                DWORD file_name_length;
                WCHAR file_name[MAX_PATH];
            };

            auto name_info = std::unique_ptr<file_name_info_t>(new (std::nothrow) file_name_info_t());
            if (!name_info)
                return false;

            // Check pipe name is templte of
            // {"cygwin-","msys-"}XXXXXXXXXXXXXXX-ptyX-XX
            if (!file_information_handler(h, FileNameInfo, name_info.get(), sizeof(file_name_info_t)))
                return false;


            // Search for cygwin or msys in the name
            std::wstring name(name_info->file_name, name_info->file_name_length / sizeof(WCHAR));
            if (name.find(L"msys-") == std::wstring::npos
                && name.find(L"cygwin-") == std::wstring::npos
                || name.find(L"-pty") == std::wstring::npos)
                return false;
            return true;
        }

        inline HANDLE get_console_handle(const std::streambuf* stream) noexcept
        {
            if (stream == std::cout.rdbuf())
            {
                static const HANDLE h_std_out = GetStdHandle(STD_OUTPUT_HANDLE);
                return h_std_out;
            }
            else if (stream == std::cerr.rdbuf() || stream == std::clog.rdbuf())
            {
                static const HANDLE h_err_out = GetStdHandle(STD_ERROR_HANDLE);
                return h_err_out;
            }
            return INVALID_HANDLE_VALUE;
        }

        inline bool set_win_term_ansi_colors(const std::streambuf* stream) noexcept
        {
            auto h = get_console_handle(stream);
            if (h == INVALID_HANDLE_VALUE)
                return false;

            DWORD mode = 0;
            if (!GetConsoleMode(h, &mode))
                return false;

            mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            if (!SetConsoleMode(h, mode))
                return false;
            return true;
        }

        inline bool support_ansi(const std::streambuf* stream) noexcept
        {
            if (stream == std::cout.rdbuf())
            {
                static const bool cout_ansi =
                    (is_msys_pty(_fileno(stdout))) || set_win_term_ansi_colors(stream);
                return cout_ansi;
            }
            else if (stream == std::cerr.rdbuf() || stream == std::clog.rdbuf())
            {
                static const bool cerr_ansi =
                    (is_msys_pty(_fileno(stderr))) || set_win_term_ansi_colors(stream);
                return cerr_ansi;
            }
            return false;
        }
    }
#endif
}}
