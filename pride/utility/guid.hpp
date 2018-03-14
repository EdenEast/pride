
#pragma once

#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

namespace pride
{
    struct guid_t
    {
        using byte_array = std::array<uint8_t, 16>;
        union
        {
            struct
            {
                uint64_t ab;
                uint64_t cd;
            };

            std::array<uint8_t, 16> bytes;
        };

        bool operator==(const guid_t& other);
        bool operator!=(const guid_t& other);
        bool operator<(const guid_t& other);

        std::string str() const;
        operator std::string() const;

        template<typename ostream>
        friend ostream& operator<<(ostream& os, const guid_t& self)
        {
            return os << self.str();
        }

        static guid_t create();
    };

    inline guid_t guid_t::create()
    {
        static std::random_device rd;
        static std::uniform_int_distribution<uint64_t> dist(0, (uint64_t)(~0));

        guid_t uid;

        uid.ab = dist(rd);
        uid.cd = dist(rd);

        uid.ab = (uid.ab & 0xFFFFFFFFFFFF0FFFULL) | 0x0000000000004000ULL;
        uid.cd = (uid.cd & 0x3FFFFFFFFFFFFFFFULL) | 0x8000000000000000ULL;

        return uid;
    }

    inline std::string guid_t::str() const
    {
        std::stringstream ss;
        ss << std::hex << std::nouppercase << std::setfill('0');

        uint32_t a = (ab >> 32);
        uint32_t b = (ab & 0xFFFFFFFF);
        uint32_t c = (cd >> 32);
        uint32_t d = (cd & 0xFFFFFFFF);

        ss << std::setw(8) << (a) << '-';
        ss << std::setw(4) << (b >> 16) << '-';
        ss << std::setw(4) << (b & 0xFFFF) << '-';
        ss << std::setw(4) << (c >> 16) << '-';
        ss << std::setw(4) << (c & 0xFFFF) << '-';
        ss << std::setw(8) << (d);

        return ss.str();
    }

    inline guid_t::operator std::string() const
    {
        return str();
    }

    inline bool guid_t::operator==(const guid_t& other)
    {
        return bytes == other.bytes;
    }

    inline bool guid_t::operator!=(const guid_t& other)
    {
        return bytes != other.bytes;
    }

    inline bool guid_t::operator<(const guid_t& other)
    {
        if (ab < other.ab)
            return true;
        if (ab > other.ab)
            return false;
        if (cd < other.cd)
            return true;
        return false;
    }
}
