
#pragma once

#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

namespace pride
{
    struct uint128_t;
}

namespace std
{
    template <> struct is_arithmetic <pride::uint128_t> : std::true_type {};
    template <> struct is_integral   <pride::uint128_t> : std::true_type {};
    template <> struct is_unsigned   <pride::uint128_t> : std::true_type {};
};

namespace pride
{
    struct uint128_t
    {
    private:
        std::pair<uint128_t, uint128_t> divmod(const uint128_t& lhs, const uint128_t& rhs) const;

    public:
        uint64_t hi = 0;
        uint64_t lo = 0;

        // Constructors
        uint128_t() = default;
        uint128_t(const uint128_t& rhs) = default;
        uint128_t(uint128_t&& rhs)
        {
            hi = std::move(rhs.hi);
            lo = std::move(rhs.lo);
            rhs.hi = 0;
            rhs.lo = 0;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint128_t(const T& rhs)
        : hi(0), lo(rhs)
        {}

        template <typename S, typename T, typename = std::enable_if_t <std::is_integral<S>::value && std::is_integral<T>::value>>
        uint128_t(const S& hi_rhs, const T& lower_rhs)
        : hi(hi_rhs), lo(lower_rhs)
        {}

        // Assignment operators
        uint128_t& operator=(const uint128_t& rhs) = default;
        uint128_t& operator=(uint128_t&& rhs) = default;

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint128_t& operator=(const T& rhs)
        {
            hi = 0;
            lo = rhs;
            return *this;
        }

        // Typecast Operators
        operator bool()     const { return static_cast<bool>(hi | lo); }
        operator uint8_t()  const { return static_cast<uint8_t> (lo); }
        operator uint16_t() const { return static_cast<uint16_t>(lo); }
        operator uint32_t() const { return static_cast<uint32_t>(lo); }
        operator uint64_t() const { return static_cast<uint64_t>(lo); }

        // Bitwise Operators
        uint128_t operator&(const uint128_t& rhs) const
        {
            return {hi & rhs.hi, lo & rhs.lo};
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint128_t operator&(const T& rhs) const
        {
            return uint128_t{0, lo & static_cast<uint64_t>(rhs)};
        }

        uint128_t& operator&=(const uint128_t& rhs)
        {
            hi &= rhs.hi;
            lo &= rhs.lo;
            return *this;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint128_t& operator&=(const T& rhs)
        {
            hi = 0;
            lo &= rhs;
            return *this;
        }

        uint128_t operator|(const uint128_t& rhs) const
        {
            return uint128_t{hi | rhs.hi, lo | rhs.lo};
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint128_t operator|(const T& rhs) const
        {
            return uint128_t{hi, lo | static_cast<uint64_t>(rhs)};
        }

        uint128_t& operator|=(const uint128_t& rhs)
        {
            hi |= rhs.hi;
            lo |= rhs.lo;
            return *this;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint128_t& operator|=(const T& rhs)
        {
            lo |= static_cast<uint64_t>(rhs);
            return *this;
        }

        uint128_t operator^(const uint128_t& rhs) const
        {
            return uint128_t{hi ^ rhs.hi, lo ^ rhs.lo};
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint128_t operator^(const T& rhs) const
        {
            return uint128_t{hi, lo ^ static_cast<uint64_t>(rhs)};
        }

        uint128_t& operator^=(const uint128_t& rhs)
        {
            hi ^= rhs.hi;
            lo ^= rhs.lo;
            return *this;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint128_t& operator^=(const T& rhs)
        {
            lo ^= static_cast<uint64_t>(rhs);
            return *this;
        }

        uint128_t operator~() const
        {
            return uint128_t{~hi, ~lo};
        }

        // Bit Shift Operators
        uint128_t operator<<(const uint128_t& rhs) const;

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint128_t operator<<(const T& rhs) const
        {
            return *this << uint128_t(rhs);
        }

        uint128_t& operator<<=(const uint128_t& rhs)
        {
            *this = *this << rhs;
            return *this;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint128_t& operator<<=(const T& rhs)
        {
            *this = *this << uint128_t(rhs);
            return *this;
        }

        uint128_t operator>>(const uint128_t& rhs) const;

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint128_t operator>>(const T& rhs) const
        {
            return *this >> uint128_t(rhs);
        }

        uint128_t& operator>>=(const uint128_t& rhs)
        {
            *this = *this >> rhs;
            return *this;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint128_t& operator>>=(const T& rhs)
        {
            *this = *this >> uint128_t(rhs);
            return *this;
        }

        // Logical Operators
        bool operator!() const { return !static_cast<bool>(hi | lo); }
        bool operator&&(const uint128_t& rhs) const { return static_cast<bool>(static_cast<bool>(*this) && rhs); }
        bool operator||(const uint128_t& rhs) const{ return static_cast<bool>(static_cast<bool>(*this) || rhs); }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        bool operator&&(const T& rhs)
        {
            return static_cast <bool> (*this && rhs);
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        bool operator||(const T& rhs)
        {
            return static_cast <bool> (*this || rhs);
        }

        // Comparison Operators
        bool operator==(const uint128_t& rhs) const
        {
            return ((hi == rhs.hi) && (lo == rhs.lo));
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        bool operator==(const T& rhs) const
        {
            return (!hi && (lo == static_cast<uint64_t>(rhs)));
        }

        bool operator!=(const uint128_t& rhs) const
        {
            return ((hi != rhs.hi) || (lo != rhs.lo));
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        bool operator!=(const T& rhs) const
        {
            return (hi | (lo != static_cast<uint64_t>(rhs)));
        }

        bool operator>(const uint128_t& rhs) const
        {
            return hi == rhs.hi ? lo > rhs.lo : hi > rhs.hi;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        bool operator>(const T& rhs) const
        {
            return (hi || (lo > static_cast<uint64_t>(rhs)));
        }

        bool operator<(const uint128_t& rhs) const
        {
            return hi == rhs.hi ? lo > rhs.lo : hi > rhs.hi;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        bool operator<(const T& rhs) const
        {
            return (!hi)?(lo < static_cast<uint64_t>(rhs)):false;
        }

        bool operator>=(const uint128_t& rhs) const
        {
            return ((*this > rhs) | (*this == rhs));
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        bool operator>=(const T& rhs) const
        {
            return ((*this > rhs) | (*this == rhs));
        }

        bool operator<=(const uint128_t& rhs) const
        {
            return ((*this < rhs) | (*this == rhs));
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        bool operator<=(const T& rhs) const
        {
            return ((*this < rhs) | (*this == rhs));
        }

        // Arithmetic Operators
        uint128_t operator+(const uint128_t& rhs) const
        {
            return uint128_t{hi + rhs.hi + ((lo + rhs.lo) < lo), lo + rhs.lo};
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint128_t operator+(const T& rhs) const
        {
            return uint128_t{hi + ((lo + static_cast<uint64_t>(rhs)) < lo), lo + static_cast<uint64_t>(rhs)};
        }

        uint128_t& operator+=(const uint128_t& rhs)
        {
            hi += rhs.hi + ((lo + rhs.lo) < lo);
            lo += rhs.lo;
            return *this;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint128_t& operator+=(const T& rhs)
        {
            hi = hi + ((lo + rhs) < lo);
            lo = lo + rhs;
            return *this;
        }

        uint128_t operator-(const uint128_t& rhs) const
        {
            return uint128_t{hi - rhs.hi - ((lo - rhs.lo) > lo), lo - rhs.lo};
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint128_t operator-(const T& rhs) const
        {
            return uint128_t{static_cast<uint64_t>(hi - ((lo - rhs) > lo)), static_cast<uint64_t>(lo - rhs)};
        }

        uint128_t& operator-=(const uint128_t& rhs)
        {
            *this = *this - rhs;
            return *this;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint128_t& operator-=(const T& rhs)
        {
            *this = *this - rhs;
            return *this;
        }

        uint128_t operator*(const uint128_t& rhs) const;

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint128_t operator*(const T& rhs) const
        {
            return *this * uint128_t(rhs);
        }

        uint128_t& operator*=(const uint128_t& rhs)
        {
            *this = *this * rhs;
            return *this;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint128_t& operator*=(const T& rhs)
        {
            *this = *this * uint128_t(rhs);
            return *this;
        }

        uint128_t operator/(const uint128_t& rhs) const
        {
            return divmod(*this, rhs).first;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint128_t operator/(const T& rhs) const{
            return *this / uint128_t(rhs);
        }

        uint128_t& operator/=(const uint128_t& rhs)
        {
            *this = *this / rhs;
            return *this;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint128_t& operator/=(const T& rhs)
        {
            *this = *this / uint128_t(rhs);
            return *this;
        }

        uint128_t operator%(const uint128_t& rhs) const
        {
            return divmod(*this, rhs).second;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint128_t operator%(const T& rhs) const{
            return *this % uint128_t(rhs);
        }

        uint128_t& operator%=(const uint128_t& rhs)
        {
            *this = *this % rhs;
            return *this;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint128_t& operator%=(const T& rhs)
        {
            *this = *this % uint128_t(rhs);
            return *this;
        }

        // Increment Operator
        uint128_t& operator++()
        {
            return *this += static_cast<uint128_t>(1);
        }

        uint128_t operator++(int)
        {
            auto temp = *this;
            ++*this;
            return temp;
        }

        // Decrement Operator
        uint128_t& operator--()
        {
            return *this -= static_cast<uint128_t>(1);
        }

        uint128_t operator--(int)
        {
            auto temp = *this;
            --*this;
            return temp;
        }

        uint128_t operator+() const
        {
            return *this;
        }

        // two's complement
        uint128_t operator-() const
        {
            return ~*this + static_cast<uint128_t>(1);
        }

        // Get bitsize of value
        uint8_t bits() const;

        // Get string representation of value
        std::string str(uint8_t base = 10, const unsigned int& len = 0) const;
    };

    //
    // ─── LEFT HAND SIZE AS T ────────────────────────────────────────────────────────
    //

    // Bitwise Operators
    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    uint128_t operator&(const T& lhs, const uint128_t& rhs)
    {
        return rhs & lhs;
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    T& operator&=(T& lhs, const uint128_t& rhs)
    {
        return lhs = static_cast <T> (rhs & lhs);
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    uint128_t operator|(const T& lhs, const uint128_t& rhs)
    {
        return rhs | lhs;
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    T& operator|=(T& lhs, const uint128_t& rhs)
    {
        return lhs = static_cast <T> (rhs | lhs);
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    uint128_t operator^(const T& lhs, const uint128_t& rhs)
    {
        return rhs ^ lhs;
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    T& operator^=(T& lhs, const uint128_t& rhs)
    {
        return lhs = static_cast <T> (rhs ^ lhs);
    }

    // Bitshift Operators
    inline uint128_t operator<<(const bool    & lhs, const uint128_t& rhs) { return uint128_t(lhs) << rhs; }
    inline uint128_t operator<<(const uint8_t & lhs, const uint128_t& rhs) { return uint128_t(lhs) << rhs; }
    inline uint128_t operator<<(const uint16_t& lhs, const uint128_t& rhs) { return uint128_t(lhs) << rhs; }
    inline uint128_t operator<<(const uint32_t& lhs, const uint128_t& rhs) { return uint128_t(lhs) << rhs; }
    inline uint128_t operator<<(const uint64_t& lhs, const uint128_t& rhs) { return uint128_t(lhs) << rhs; }
    inline uint128_t operator<<(const int8_t  & lhs, const uint128_t& rhs) { return uint128_t(lhs) << rhs; }
    inline uint128_t operator<<(const int16_t & lhs, const uint128_t& rhs) { return uint128_t(lhs) << rhs; }
    inline uint128_t operator<<(const int32_t & lhs, const uint128_t& rhs) { return uint128_t(lhs) << rhs; }
    inline uint128_t operator<<(const int64_t & lhs, const uint128_t& rhs) { return uint128_t(lhs) << rhs; }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    T& operator<<=(T& lhs, const uint128_t& rhs)
    {
        return lhs = static_cast<T>(uint128_t(lhs) << rhs);
    }

    inline uint128_t operator>>(const bool    & lhs, const uint128_t& rhs) { return uint128_t(lhs) >> rhs; }
    inline uint128_t operator>>(const uint8_t & lhs, const uint128_t& rhs) { return uint128_t(lhs) >> rhs; }
    inline uint128_t operator>>(const uint16_t& lhs, const uint128_t& rhs) { return uint128_t(lhs) >> rhs; }
    inline uint128_t operator>>(const uint32_t& lhs, const uint128_t& rhs) { return uint128_t(lhs) >> rhs; }
    inline uint128_t operator>>(const uint64_t& lhs, const uint128_t& rhs) { return uint128_t(lhs) >> rhs; }
    inline uint128_t operator>>(const int8_t  & lhs, const uint128_t& rhs) { return uint128_t(lhs) >> rhs; }
    inline uint128_t operator>>(const int16_t & lhs, const uint128_t& rhs) { return uint128_t(lhs) >> rhs; }
    inline uint128_t operator>>(const int32_t & lhs, const uint128_t& rhs) { return uint128_t(lhs) >> rhs; }
    inline uint128_t operator>>(const int64_t & lhs, const uint128_t& rhs) { return uint128_t(lhs) >> rhs; }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    T& operator>>=(T& lhs, const uint128_t& rhs)
    {
        return lhs = static_cast<T>(uint128_t(lhs) >> rhs);
    }

    // Comparison Operators
    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    bool operator==(const T& lhs, const uint128_t& rhs)
    {
        return (!rhs.hi && (static_cast<uint64_t>(lhs) == rhs.lo));
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    bool operator!=(const T& lhs, const uint128_t& rhs)
    {
        return (rhs.hi | (static_cast<uint64_t>(lhs) != rhs.lo));
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    bool operator>(const T& lhs, const uint128_t& rhs)
    {
        return (!rhs.hi) && (static_cast<uint64_t>(lhs) > rhs.lo);
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    bool operator<(const T& lhs, const uint128_t& rhs)
    {
        if (rhs.hi)
            return true;
        return (static_cast<uint64_t>(lhs) < rhs.lo);
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    bool operator>=(const T& lhs, const uint128_t& rhs)
    {
        if (rhs.hi)
            return false;
        return (static_cast<uint64_t>(lhs) >= rhs.lo);
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    bool operator<=(const T& lhs, const uint128_t& rhs)
    {
        if (rhs.hi)
            return true;
        return (static_cast<uint64_t>(lhs) <= rhs.lo);
    }

    // Arithmetic Operators
    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    uint128_t operator+(const T& lhs, const uint128_t& rhs)
    {
        return rhs + lhs;
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    T& operator+=(T& lhs, const uint128_t& rhs)
    {
        return lhs = static_cast <T> (rhs + lhs);
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    uint128_t operator-(const T& lhs, const uint128_t& rhs)
    {
        return -(rhs - lhs);
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    T& operator-=(T& lhs, const uint128_t& rhs)
    {
        return lhs = static_cast <T> (-(rhs - lhs));
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    uint128_t operator*(const T& lhs, const uint128_t& rhs)
    {
        return rhs * lhs;
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    T& operator*=(T& lhs, const uint128_t& rhs)
    {
        return lhs = static_cast <T> (rhs * lhs);
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    uint128_t operator/(const T& lhs, const uint128_t& rhs)
    {
        return uint128_t(lhs) / rhs;
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    T& operator/=(T& lhs, const uint128_t& rhs)
    {
        return lhs = static_cast <T> (uint128_t(lhs) / rhs);
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    uint128_t operator%(const T& lhs, const uint128_t& rhs)
    {
        return uint128_t(lhs) % rhs;
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    T& operator%=(T& lhs, const uint128_t& rhs)
    {
        return lhs = static_cast <T> (uint128_t(lhs) % rhs);
    }

    // IO Operator
    inline std::ostream & operator<<(std::ostream& stream, const uint128_t& rhs)
    {
        if (stream.flags() & stream.oct)
            stream << rhs.str(8);
        else if (stream.flags() & stream.dec)
            stream << rhs.str(10);
        else if (stream.flags() & stream.hex)
            stream << rhs.str(16);
        return stream;
    }

    //
    // ─── IMPLEMENTATION ─────────────────────────────────────────────────────────────
    //

    inline uint128_t uint128_t::operator<<(const uint128_t& rhs) const
    {
        const uint64_t shift = rhs.lo;

        if (static_cast<bool>(rhs.hi) || shift >= 128)
            return static_cast<uint128_t>(0);
        else if (shift == 64)
            return {lo, 0};
        else if (shift == 0)
            return *this;
        else if (shift < 64)
            return {(hi << shift) + (lo >> (64 - shift)), lo << shift};
        else if ((128 > shift) && (shift > 64))
            return {lo << (shift - 64), 0};
        else
            return 0;
    }

    inline uint128_t uint128_t::operator>>(const uint128_t& rhs) const
    {
        const uint64_t shift = rhs.lo;
        if (((bool) rhs.hi) || (shift >= 128))
            return static_cast<uint128_t>(0);
        else if (shift == 64)
            return {0, hi};
        else if (shift == 0)
            return *this;
        else if (shift < 64)
            return {hi >> shift, (hi << (64 - shift)) + (lo >> shift)};
        else if ((128 > shift) && (shift > 64))
            return {0, (hi >> (shift - 64))};
        else
            return static_cast<uint128_t>(0);
    }

    inline uint128_t uint128_t::operator*(const uint128_t& rhs) const
    {
        // split into 4 * 32 bit values
        uint64_t top[4] = {hi >> 32, hi & 0xffffffff, lo >> 32, lo & 0xffffffff};
        uint64_t bottom[4] = {rhs.hi >> 32, rhs.hi & 0xffffffff, rhs.lo >> 32, rhs.lo & 0xffffffff};
        uint64_t products[4][4];

        // multiply each component of the values
        for(int y = 3; y > -1; y--)
            for(int x = 3; x > -1; x--)
                products[3 - x][y] = top[x] * bottom[y];

         // first row
        uint64_t fourth32 = (products[0][3] & 0xffffffff);
        uint64_t third32  = (products[0][2] & 0xffffffff) + (products[0][3] >> 32);
        uint64_t second32 = (products[0][1] & 0xffffffff) + (products[0][2] >> 32);
        uint64_t first32  = (products[0][0] & 0xffffffff) + (products[0][1] >> 32);

        // second row
        third32  += (products[1][3] & 0xffffffff);
        second32 += (products[1][2] & 0xffffffff) + (products[1][3] >> 32);
        first32  += (products[1][1] & 0xffffffff) + (products[1][2] >> 32);

        // third row
        second32 += (products[2][3] & 0xffffffff);
        first32  += (products[2][2] & 0xffffffff) + (products[2][3] >> 32);

        // fourth row
        first32  += (products[3][3] & 0xffffffff);

        // move carry to next digit
        third32  += fourth32 >> 32;
        second32 += third32  >> 32;
        first32  += second32 >> 32;

        // remove carry from current digit
        fourth32 &= 0xffffffff;
        third32  &= 0xffffffff;
        second32 &= 0xffffffff;
        first32  &= 0xffffffff;

        // combine components
        return uint128_t((first32 << 32) | second32, (third32 << 32) | fourth32);
    }

    inline std::pair<uint128_t, uint128_t> uint128_t::divmod(const uint128_t& lhs, const uint128_t& rhs) const
    {
        static const uint128_t zero = static_cast<uint128_t>(0);
        static const uint128_t one = static_cast<uint128_t>(1);

        if (rhs == zero)
            throw std::domain_error("Error: division or modulus by 0");

        if (rhs == one)
            return std::pair <uint128_t, uint128_t> (lhs, zero);
        else if (lhs == rhs)
            return std::pair <uint128_t, uint128_t> (one, zero);
        else if ((lhs == zero) || (lhs < rhs))
            return std::pair <uint128_t, uint128_t> (zero, lhs);

        std::pair <uint128_t, uint128_t> pair(zero, zero);
        for(uint8_t x = lhs.bits(); x > 0; x--)
        {
            pair.first  <<= one;
            pair.second <<= one;

            if ((lhs >> (x - 1U)) & 1)
                pair.second++;

            if (pair.second >= rhs)
            {
                pair.second -= rhs;
                pair.first++;
            }
        }
        return pair;
    }

    inline uint8_t uint128_t::bits() const
    {
        uint8_t out = 0;
        if (hi)
        {
            out = 64;
            uint64_t up = hi;
            while (up)
            {
                up >>= 1;
                out++;
            }
        }
        else
        {
            uint64_t low = lo;
            while (low)
            {
                low >>= 1;
                out++;
            }
        }
        return out;
    }

    inline std::string uint128_t::str(uint8_t base, const unsigned int& len) const
    {
        static const uint128_t zero = static_cast<uint128_t>(0);

        if ((base < 2) || (base > 16))
            throw std::invalid_argument("Base must be in the range [2, 16]");

        std::string out = "";
        if (!(*this))
        {
            out = "0";
        }
        else
        {
            std::pair<uint128_t, uint128_t> pair(*this, zero);
            do
            {
                pair = divmod(pair.first, base);
                out = "0123456789abcdef"[(uint8_t)pair.second] + out;
            }
            while(pair.first);
        }

        if (out.size() < len)
            out = std::string(len - out.size(), '0') + out;

        return out;
    }
}
