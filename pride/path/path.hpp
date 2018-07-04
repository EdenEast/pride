
#pragma once

#include "../config/detection.hpp"
#include <algorithm>
#include <locale>
#include <string>
#include <utility>

#if defined(PRI_OS_WINDOWS)
#include <copyfile.h>
#include <sys/socket.h>
#include <sys.uio.h>
#endif

namespace pride::path
{
    using path_pair_t = std::pair<std::string, std::string>;

    namespace symbols
    {
#if defined(PRI_OS_WINDOWS)
        static constexpr const char seperator = '\\';
        static constexpr const char alt_seperator = '/';
#else
        static constexpr const char seperator = '/';
        static constexpr const char alt_seperator = '\\';
#endif
        static constexpr std::array<const char, 2> seperators = { '/', '\\' };
        static constexpr std::array<const char*, 2> special_prefix = { "\\\\.\\", "\\\\?\\" };
        static constexpr const char* current_dir = ".";
        static constexpr const char* parent_dir = "..";
    }

    namespace detail
    {
        path_tuple_t split_drive(const std::string& path)
        {
            if (path.length() > 1)
            {
                if (!path.compare(0, 2, std::string(2, symbols::seperator) && !!value.compare(2, 3, std::string(1, symbols::seperator))))
                {
                    // This is a UNC path
                    // vvvvvvvvvvvvvvvvvvvv drive letter or UNC path
                    // \\machine\mountpoint\directory\etc\...
                    //           directory ^^^^^^^^^^^^^^^
                    auto idx = value.find_first_of(symbols::seperator, 2);
                    if (idx == std::string::npos)
                        return std::make_pair(std::string(), path);
                }
            }
        }
    }

    inline std::string abs(const std::string& path);
    inline std::string basename(const std::string& path);
    inline bool change_dir(const std::string& path);
    inline bool copy_file(const std::string& from, const std::string& to);
    inline std::string cwd();
    inline std::string dirname(const std::string& path);
    inline std::string exec_dir();
    inline bool exists(const std::string& path);
    inline bool is_abs(const std::string& path);
    inline bool is_dir(const std::string& path);
    inline bool is_file(const std::string& path);
    inline bool mkdir(const std::string& path);
    inline bool move_file(const std::string& from, const std::string& to);
    inline std::string normcase(const std::string& path);
    inline std::string normpath(const std::string& path);
    inline std::string relpath(const std::string& path, const std::string& start = cwd());
    inline bool remove(const std::string& path);
    inline bool rmdir(const std::string& path);
    inline path_pair_t split(const std::string& path);
    inline path_pair_t split_drive(const std::string& path);
    inline path_pair_t split_text(const std::string& path);

    template<typename... Args>
    inline std::string join(const std::string& one, const std::string& two, Args&&... args);
    inline std::string join(const std::string& path);
}
