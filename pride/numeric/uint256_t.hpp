
#pragma once

#include "uint128_t.hpp"
#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

namespace pride
{
    struct uint256_t;
}

namespace std
{
    template <> struct is_arithmetic <pride::uint256_t> : std::true_type {};
    template <> struct is_integral   <pride::uint256_t> : std::true_type {};
    template <> struct is_unsigned   <pride::uint256_t> : std::true_type {};
};

namespace pride
{
    struct uint256_t
    {
    private:
    std::pair<uint256_t, uint256_t> divmod(const uint256_t& lhs, const uint256_t& rhs) const;

    public:
        uint128_t hi = 0;
        uint128_t lo = 0;

        uint256_t() = default;
        uint256_t(const uint256_t& rhs) = default;
        uint256_t(uint256_t&& rhs)
        {
            hi = std::move(rhs.hi);
            lo = std::move(rhs.lo);
            rhs.hi = 0;
            rhs.lo = 0;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint256_t(const T& rhs)
        : hi(0), lo(rhs)
        {}

        template <typename S, typename T, typename = std::enable_if_t <std::is_integral<S>::value && std::is_integral<T>::value>>
        uint256_t(const S& hi_rhs, const T& lower_rhs)
        : hi(hi_rhs), lo(lower_rhs)
        {}

        template<typename S, typename T, typename U, typename V,
            typename = std::enable_if_t<
                std::is_integral<S>::value && std::is_integral<T>::value &&
                std::is_integral<U>::value && std::is_integral<V>::value
            >>
        uint256_t(const S& hi_lhs, const T& lo_lhs, const U& hi_rhs, const V& lo_rhs)
        : hi(hi_lhs, lo_lhs) , lo(hi_rhs, lo_rhs)
        {}

        // Assignment operators
        uint256_t& operator=(const uint256_t& rhs) = default;
        uint256_t& operator=(uint256_t&& rhs) = default;

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint256_t& operator=(const T& rhs)
        {
            hi = 0;
            lo = rhs;
            return *this;
        }

        // Typecast Operators
        operator bool()      const { return static_cast<bool>(hi | lo); }
        operator uint8_t()   const { return static_cast<uint8_t> (lo);  }
        operator uint16_t()  const { return static_cast<uint16_t>(lo);  }
        operator uint32_t()  const { return static_cast<uint32_t>(lo);  }
        operator uint64_t()  const { return static_cast<uint64_t>(lo);  }
        operator uint128_t() const { return static_cast<uint128_t>(lo); }

        // Bitwise Operators
        uint256_t operator&(const uint256_t& rhs) const
        {
            return {hi & rhs.hi, lo & rhs.lo};
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint256_t operator&(const T& rhs) const
        {
            return uint256_t{0, lo & static_cast<uint128_t>(rhs)};
        }

        uint256_t& operator&=(const uint256_t& rhs)
        {
            hi &= rhs.hi;
            lo &= rhs.lo;
            return *this;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint256_t& operator&=(const T& rhs)
        {
            hi = 0;
            lo &= rhs;
            return *this;
        }

        uint256_t operator|(const uint256_t& rhs) const
        {
            return uint256_t{hi | rhs.hi, lo | rhs.lo};
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint256_t operator|(const T& rhs) const
        {
            return uint256_t{hi, lo | static_cast<uint128_t>(rhs)};
        }

        uint256_t& operator|=(const uint256_t& rhs)
        {
            hi |= rhs.hi;
            lo |= rhs.lo;
            return *this;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint256_t& operator|=(const T& rhs)
        {
            lo |= static_cast<uint128_t>(rhs);
            return *this;
        }

        uint256_t operator^(const uint256_t& rhs) const
        {
            return uint256_t{hi ^ rhs.hi, lo ^ rhs.lo};
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint256_t operator^(const T& rhs) const
        {
            return uint256_t{hi, lo ^ static_cast<uint128_t>(rhs)};
        }

        uint256_t& operator^=(const uint256_t& rhs)
        {
            hi ^= rhs.hi;
            lo ^= rhs.lo;
            return *this;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint256_t& operator^=(const T& rhs)
        {
            lo ^= static_cast<uint128_t>(rhs);
            return *this;
        }

        uint256_t operator~() const
        {
            return uint256_t{~hi, ~lo};
        }

        // Bit Shift Operators
        uint256_t operator<<(const uint256_t& rhs) const;

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint256_t operator<<(const T& rhs) const
        {
            return *this << uint256_t(rhs);
        }

        uint256_t& operator<<=(const uint256_t& rhs)
        {
            *this = *this << rhs;
            return *this;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint256_t& operator<<=(const T& rhs)
        {
            *this = *this << uint256_t(rhs);
            return *this;
        }

        uint256_t operator>>(const uint256_t& rhs) const;

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint256_t operator>>(const T& rhs) const
        {
            return *this >> uint256_t(rhs);
        }

        uint256_t& operator>>=(const uint256_t& rhs)
        {
            *this = *this >> rhs;
            return *this;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint256_t& operator>>=(const T& rhs)
        {
            *this = *this >> uint256_t(rhs);
            return *this;
        }

        // Logical Operators
        bool operator!() const { return !static_cast<bool>(hi | lo); }
        bool operator&&(const uint256_t& rhs) const { return static_cast<bool>(*this && rhs); }
        bool operator||(const uint256_t& rhs) const{ return static_cast<bool>(*this || rhs); }

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
        bool operator==(const uint256_t& rhs) const
        {
            return ((hi == rhs.hi) && (lo == rhs.lo));
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        bool operator==(const T& rhs) const
        {
            return (!hi && (lo == static_cast<uint128_t>(rhs)));
        }

        bool operator!=(const uint256_t& rhs) const
        {
            return ((hi != rhs.hi) || (lo != rhs.lo));
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        bool operator!=(const T& rhs) const
        {
            return (hi | (lo != static_cast<uint128_t>(rhs)));
        }

        bool operator>(const uint256_t& rhs) const
        {
            return hi == rhs.hi ? lo > rhs.lo : hi > rhs.hi;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        bool operator>(const T& rhs) const
        {
            return (hi || (lo > static_cast<uint128_t>(rhs)));
        }

        bool operator<(const uint256_t& rhs) const
        {
            return hi == rhs.hi ? lo > rhs.lo : hi > rhs.hi;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        bool operator<(const T& rhs) const
        {
            return (!hi)?(lo < static_cast<uint128_t>(rhs)):false;
        }

        bool operator>=(const uint256_t& rhs) const
        {
            return ((*this > rhs) | (*this == rhs));
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        bool operator>=(const T& rhs) const
        {
            return ((*this > rhs) | (*this == rhs));
        }

        bool operator<=(const uint256_t& rhs) const
        {
            return ((*this < rhs) | (*this == rhs));
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        bool operator<=(const T& rhs) const
        {
            return ((*this < rhs) | (*this == rhs));
        }

        // Arithmetic Operators
        uint256_t operator+(const uint256_t& rhs) const
        {
            return uint256_t{hi + rhs.hi + ((lo + rhs.lo) < lo), lo + rhs.lo};
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint256_t operator+(const T& rhs) const
        {
            return uint256_t{hi + ((lo + static_cast<uint128_t>(rhs)) < lo), lo + static_cast<uint128_t>(rhs)};
        }

        uint256_t& operator+=(const uint256_t& rhs)
        {
            hi += rhs.hi + ((lo + rhs.lo) < lo);
            lo += rhs.lo;
            return *this;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint256_t& operator+=(const T& rhs)
        {
            hi = hi + ((lo + rhs) < lo);
            lo = lo + rhs;
            return *this;
        }

        uint256_t operator-(const uint256_t& rhs) const
        {
            return uint256_t{hi - rhs.hi - ((lo - rhs.lo) > lo), lo - rhs.lo};
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint256_t operator-(const T& rhs) const
        {
            return uint256_t{static_cast<uint128_t>(hi - ((lo - rhs) > lo)), static_cast<uint128_t>(lo - rhs)};
        }

        uint256_t& operator-=(const uint256_t& rhs)
        {
            *this = *this - rhs;
            return *this;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint256_t& operator-=(const T& rhs)
        {
            *this = *this - rhs;
            return *this;
        }

        uint256_t operator*(const uint256_t& rhs) const;

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint256_t operator*(const T& rhs) const
        {
            return *this * uint256_t(rhs);
        }

        uint256_t& operator*=(const uint256_t& rhs)
        {
            *this = *this * rhs;
            return *this;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint256_t& operator*=(const T& rhs)
        {
            *this = *this * uint256_t(rhs);
            return *this;
        }

        uint256_t operator/(const uint256_t& rhs) const
        {
            return divmod(*this, rhs).first;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint256_t operator/(const T& rhs) const{
            return *this / uint256_t(rhs);
        }

        uint256_t& operator/=(const uint256_t& rhs)
        {
            *this = *this / rhs;
            return *this;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint256_t& operator/=(const T& rhs)
        {
            *this = *this / uint256_t(rhs);
            return *this;
        }

        uint256_t operator%(const uint256_t& rhs) const
        {
            return divmod(*this, rhs).second;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint256_t operator%(const T& rhs) const{
            return *this % uint256_t(rhs);
        }

        uint256_t& operator%=(const uint256_t& rhs)
        {
            *this = *this % rhs;
            return *this;
        }

        template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
        uint256_t& operator%=(const T& rhs)
        {
            *this = *this % uint256_t(rhs);
            return *this;
        }

        // Increment Operator
        uint256_t& operator++()
        {
            return *this += static_cast<uint256_t>(1);
        }

        uint256_t operator++(int)
        {
            auto temp = *this;
            ++*this;
            return temp;
        }

        // Decrement Operator
        uint256_t& operator--()
        {
            return *this -= static_cast<uint256_t>(1);
        }

        uint256_t operator--(int)
        {
            auto temp = *this;
            --*this;
            return temp;
        }

        uint256_t operator+() const
        {
            return *this;
        }

        // two's complement
        uint256_t operator-() const
        {
            return ~*this + static_cast<uint256_t>(1);
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
    uint256_t operator&(const T& lhs, const uint256_t& rhs)
    {
        return rhs & lhs;
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    T& operator&=(T& lhs, const uint256_t& rhs)
    {
        return lhs = static_cast <T> (rhs & lhs);
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    uint256_t operator|(const T& lhs, const uint256_t& rhs)
    {
        return rhs | lhs;
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    T& operator|=(T& lhs, const uint256_t& rhs)
    {
        return lhs = static_cast <T> (rhs | lhs);
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    uint256_t operator^(const T& lhs, const uint256_t& rhs)
    {
        return rhs ^ lhs;
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    T& operator^=(T& lhs, const uint256_t& rhs)
    {
        return lhs = static_cast <T> (rhs ^ lhs);
    }

    // Bitshift Operators
    inline uint256_t operator<<(const bool     & lhs, const uint256_t& rhs) { return uint256_t(lhs) << rhs; }
    inline uint256_t operator<<(const uint8_t  & lhs, const uint256_t& rhs) { return uint256_t(lhs) << rhs; }
    inline uint256_t operator<<(const uint16_t & lhs, const uint256_t& rhs) { return uint256_t(lhs) << rhs; }
    inline uint256_t operator<<(const uint32_t & lhs, const uint256_t& rhs) { return uint256_t(lhs) << rhs; }
    inline uint256_t operator<<(const uint64_t & lhs, const uint256_t& rhs) { return uint256_t(lhs) << rhs; }
    inline uint256_t operator<<(const uint128_t& lhs, const uint256_t& rhs) { return uint256_t(lhs) << rhs; }
    inline uint256_t operator<<(const int8_t   & lhs, const uint256_t& rhs) { return uint256_t(lhs) << rhs; }
    inline uint256_t operator<<(const int16_t  & lhs, const uint256_t& rhs) { return uint256_t(lhs) << rhs; }
    inline uint256_t operator<<(const int32_t  & lhs, const uint256_t& rhs) { return uint256_t(lhs) << rhs; }
    inline uint256_t operator<<(const int64_t  & lhs, const uint256_t& rhs) { return uint256_t(lhs) << rhs; }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    T& operator<<=(T& lhs, const uint256_t& rhs)
    {
        return lhs = static_cast<T>(uint256_t(lhs) << rhs);
    }

    inline uint256_t operator>>(const bool     & lhs, const uint256_t& rhs) { return uint256_t(lhs) >> rhs; }
    inline uint256_t operator>>(const uint8_t  & lhs, const uint256_t& rhs) { return uint256_t(lhs) >> rhs; }
    inline uint256_t operator>>(const uint16_t & lhs, const uint256_t& rhs) { return uint256_t(lhs) >> rhs; }
    inline uint256_t operator>>(const uint32_t & lhs, const uint256_t& rhs) { return uint256_t(lhs) >> rhs; }
    inline uint256_t operator>>(const uint64_t & lhs, const uint256_t& rhs) { return uint256_t(lhs) >> rhs; }
    inline uint256_t operator>>(const uint128_t& lhs, const uint256_t& rhs) { return uint256_t(lhs) >> rhs; }
    inline uint256_t operator>>(const int8_t   & lhs, const uint256_t& rhs) { return uint256_t(lhs) >> rhs; }
    inline uint256_t operator>>(const int16_t  & lhs, const uint256_t& rhs) { return uint256_t(lhs) >> rhs; }
    inline uint256_t operator>>(const int32_t  & lhs, const uint256_t& rhs) { return uint256_t(lhs) >> rhs; }
    inline uint256_t operator>>(const int64_t  & lhs, const uint256_t& rhs) { return uint256_t(lhs) >> rhs; }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    T& operator>>=(T& lhs, const uint256_t& rhs)
    {
        return lhs = static_cast<T>(uint256_t(lhs) >> rhs);
    }

    // Comparison Operators
    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    bool operator==(const T& lhs, const uint256_t& rhs)
    {
        return (!rhs.hi && (static_cast<uint128_t>(lhs) == rhs.lo));
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    bool operator!=(const T& lhs, const uint256_t& rhs)
    {
        return (rhs.hi | (static_cast<uint128_t>(lhs) != rhs.lo));
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    bool operator>(const T& lhs, const uint256_t& rhs)
    {
        return (!rhs.hi) && (static_cast<uint128_t>(lhs) > rhs.lo);
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    bool operator<(const T& lhs, const uint256_t& rhs)
    {
        if (rhs.hi)
            return true;
        return (static_cast<uint128_t>(lhs) < rhs.lo);
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    bool operator>=(const T& lhs, const uint256_t& rhs)
    {
        if (rhs.hi)
            return false;
        return (static_cast<uint128_t>(lhs) >= rhs.lo);
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    bool operator<=(const T& lhs, const uint256_t& rhs)
    {
        if (rhs.hi)
            return true;
        return (static_cast<uint128_t>(lhs) <= rhs.lo);
    }

    // Arithmetic Operators
    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    uint256_t operator+(const T& lhs, const uint256_t& rhs)
    {
        return rhs + lhs;
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    T& operator+=(T& lhs, const uint256_t& rhs)
    {
        return lhs = static_cast <T> (rhs + lhs);
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    uint256_t operator-(const T& lhs, const uint256_t& rhs)
    {
        return -(rhs - lhs);
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    T& operator-=(T& lhs, const uint256_t& rhs)
    {
        return lhs = static_cast <T> (-(rhs - lhs));
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    uint256_t operator*(const T& lhs, const uint256_t& rhs)
    {
        return rhs * lhs;
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    T& operator*=(T& lhs, const uint256_t& rhs)
    {
        return lhs = static_cast <T> (rhs * lhs);
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    uint256_t operator/(const T& lhs, const uint256_t& rhs)
    {
        return uint256_t(lhs) / rhs;
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    T& operator/=(T& lhs, const uint256_t& rhs)
    {
        return lhs = static_cast <T> (uint256_t(lhs) / rhs);
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    uint256_t operator%(const T& lhs, const uint256_t& rhs)
    {
        return uint256_t(lhs) % rhs;
    }

    template <typename T, typename = std::enable_if_t <std::is_integral<T>::value>>
    T& operator%=(T& lhs, const uint256_t& rhs)
    {
        return lhs = static_cast <T> (uint256_t(lhs) % rhs);
    }

    // IO Operator
    inline std::ostream & operator<<(std::ostream& stream, const uint256_t& rhs)
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

    inline uint256_t uint256_t::operator<<(const uint256_t& rhs) const
    {
        const uint128_t shift = rhs.lo;

        if (static_cast<bool>(rhs.hi) || shift >= 258)
            return static_cast<uint256_t>(0);
        else if (shift == 128)
            return {lo, 0};
        else if (shift == 0)
            return *this;
        else if (shift < 128)
            return {(hi << shift) + (lo >> (128 - shift)), lo << shift};
        else if ((256 > shift) && (shift > 128))
            return {lo << (shift - 128), 0};
        else
            return 0;
    }

    inline uint256_t uint256_t::operator>>(const uint256_t& rhs) const
    {
        const uint128_t shift = rhs.lo;
        if (((bool) rhs.hi) || (shift >= 256))
            return static_cast<uint256_t>(0);
        else if (shift == 128)
            return {0, hi};
        else if (shift == 0)
            return *this;
        else if (shift < 128)
            return {hi >> shift, (hi << (128 - shift)) + (lo >> shift)};
        else if ((256 > shift) && (shift > 128))
            return {0, (hi >> (shift - 128))};
        else
            return static_cast<uint256_t>(0);
    }

    inline uint256_t uint256_t::operator*(const uint256_t& rhs) const
    {
        // split into 4 * 32 bit values
        uint128_t top[4] = {hi.hi, hi.lo, lo.hi, lo.lo};
        uint128_t bottom[4] = {rhs.hi.hi, rhs.hi.lo, rhs.lo.hi, rhs.lo.lo};
        uint128_t products[4][4];

        // multiply each component of the values
        for(int y = 3; y > -1; y--)
            for(int x = 3; x > -1; x--)
                products[3 - x][y] = top[x] * bottom[y];

         // first row
        uint128_t fourth64 = (products[0][3].lo);
        uint128_t third64  = (products[0][2].lo) + (products[0][3].hi);
        uint128_t second64 = (products[0][1].lo) + (products[0][2].hi);
        uint128_t first64  = (products[0][0].lo) + (products[0][1].hi);

        // second row
        third64  += (products[1][3].lo);
        second64 += (products[1][2].lo) + (products[1][3].hi);
        first64  += (products[1][1].lo) + (products[1][2].hi);

        // third row
        second64 += (products[2][3].lo);
        first64  += (products[2][2].lo) + (products[2][3].hi);

        // fourth row
        first64  += (products[3][3].lo);

        // combine components
        return uint256_t(first64 << (uint128_t) 64, (uint128_t) 0) +
           uint256_t(third64.hi, third64 << (uint128_t) 64) +
           uint256_t(second64, (uint128_t) 0) +
           uint256_t(fourth64);
    }

    inline std::pair<uint256_t, uint256_t> uint256_t::divmod(const uint256_t& lhs, const uint256_t& rhs) const
    {
        static const uint256_t zero = static_cast<uint256_t>(0);
        static const uint256_t one = static_cast<uint256_t>(1);

       if (rhs == zero)
            throw std::domain_error("Error: division or modulus by 0");

        if (rhs == one)
            return std::pair <uint256_t, uint256_t> (lhs, zero);
        else if (lhs == rhs)
            return std::pair <uint256_t, uint256_t> (one, zero);
        else if ((lhs == zero) || (lhs < rhs))
            return std::pair <uint256_t, uint256_t> (zero, lhs);

        std::pair <uint256_t, uint256_t> qr(zero, lhs);
        uint256_t copyd = rhs << (lhs.bits() - rhs.bits());
        uint256_t adder = one << (lhs.bits() - rhs.bits());
        if (copyd > qr.second)
        {
            copyd >>= one;
            adder >>= one;
        }
        while (qr.second >= rhs)
        {
            if (qr.second >= copyd){
                qr.second -= copyd;
                qr.first |= adder;
            }
            copyd >>= one;
            adder >>= one;
        }
        return qr;
   }

    inline uint8_t uint256_t::bits() const
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

    inline std::string uint256_t::str(uint8_t base, const unsigned int& len) const
    {
        static const uint256_t zero = static_cast<uint256_t>(0);

        if ((base < 2) || (base > 16))
            throw std::invalid_argument("Base must be in the range [2, 16]");

        std::string out = "";
        if (!(*this))
        {
            out = "0";
        }
        else
        {
            std::pair<uint256_t, uint256_t> pair(*this, zero);
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
