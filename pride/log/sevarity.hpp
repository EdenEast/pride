
#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <unordered_map>

namespace pride::log
{
    enum class sevarity_t // : uint8_t
    {
        trace    = 0,
        info     = 1,
        debug    = 2,
        warn     = 3,
        error    = 4,
        critical = 5,
        off      = 6
    };

    namespace detail
    {
        namespace sevarity
        {
            static constexpr std::array<const char*, 7> long_names {{
                "trace", "debug", "info", "warning", "error", "critical", "off"
            }};

            static constexpr std::array<const char*, 7> short_names {{
                "T", "D", "I", "W", "E", "C", "O"
            }};

            static std::unordered_map<const char*, sevarity_t> name_to_sevarity {{
                {long_names [0], sevarity_t::trace},
                {long_names [1], sevarity_t::debug},
                {long_names [2], sevarity_t::info},
                {long_names [3], sevarity_t::warn},
                {long_names [4], sevarity_t::error},
                {long_names [5], sevarity_t::critical},
                {long_names [6], sevarity_t::off},

                {short_names[0], sevarity_t::trace},
                {short_names[1], sevarity_t::debug},
                {short_names[2], sevarity_t::info},
                {short_names[3], sevarity_t::warn},
                {short_names[4], sevarity_t::error},
                {short_names[5], sevarity_t::critical},
                {short_names[6], sevarity_t::off},
            }};
        }

        inline constexpr const char* to_long_name(sevarity_t sevarity)
        {
            return sevarity::long_names[static_cast<uint8_t>(sevarity)];
        }

        inline constexpr const char* to_short_name(sevarity_t sevarity)
        {
            return sevarity::short_names[static_cast<uint8_t>(sevarity)];
        }

        inline sevarity_t to_sevarity(const char* name)
        {
            auto iter = sevarity::name_to_sevarity.find(name);
            return iter != sevarity::name_to_sevarity.end() ? iter->second : sevarity_t::off;
        }
    }
}
