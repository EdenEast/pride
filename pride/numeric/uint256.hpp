
#pragma once

#include "uint128.hpp"
#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace pride
{
    struct uint256_t;
}

// Give uint256_t type traits
namespace std
{
    template<> struct is_arithmetic <pride::uint256_t> : std::true_type {};
    template<> struct is_integral   <pride::uint256_t> : std::true_type {};
    template<> struct is_unsigned   <pride::uint256_t> : std::true_type {};
};

namespace pride
{
    struct uint256_t
    {
    private:
        std::pair <uint256_t, uint256_t> divmod(const uint256_t& lhs, const uint256_t& rhs) const;

    public:
        uint128_t hi, lo;

        // Constructors
        uint256_t()
        : hi(0), lo(0)
        {}

        uint256_t(const uint256_t& rhs)
        : hi(rhs.hi), lo(rhs.lo)
        {}

        uint256_t(uint256_t&& rhs)
        : hi(std::move(rhs.hi)), lo(std::move(rhs.lo))
        {
            if (this != &rhs)
            {
                rhs.hi = 0;
                rhs.lo = 0;
            }
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        uint256_t(const T& rhs)
        : hi(0), lo(rhs)
        {}

        template<typename S, typename T, typename = std::enable_if_t<std::is_integral<S>::value && std::is_integral<T>::value>>
        uint256_t(const S & upper_rhs, const T& lower_rhs)
        : hi(upper_rhs), lo(lower_rhs)
        {}

        template<typename R, typename S, typename T, typename U,
            typename = std::enable_if_t<
                std::is_integral<R>::value && std::is_integral<S>::value &&
                std::is_integral<T>::value && std::is_integral<U>::value
            >
        >
        uint256_t(const R & upper_lhs, const S & lower_lhs, const T& upper_rhs, const U & lower_rhs)
        : hi(upper_lhs, lower_lhs), lo(upper_rhs, lower_rhs)
        {}

        //  RHS input args only

        // Assignment Operator
        uint256_t& operator=(const uint256_t& rhs)
        {
            hi = rhs.hi;
            lo = rhs.lo;
            return *this;
        }
        uint256_t& operator=(uint256_t&& rhs)
        {
            if (this != &rhs)
            {
                hi = std::move(rhs.hi);
                lo = std::move(rhs.lo);
                rhs.hi = 0;
                rhs.lo = 0;
            }
            return *this;
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        uint256_t& operator=(const T& rhs)
        {
            hi = uint128_0;
            lo = rhs;
            return *this;
        }

        // Typecast Operators
        operator bool     () const { return static_cast<bool>(lo | hi); }
        operator uint8_t  () const { return static_cast<uint8_t>(lo); }
        operator uint16_t () const { return static_cast<uint16_t>(lo); }
        operator uint32_t () const { return static_cast<uint32_t>(lo); }
        operator uint64_t () const { return static_cast<uint64_t>(lo); }
        operator uint128_t() const { return lo; }

        // Bitwise Operators
        uint256_t operator&(const uint128_t& rhs) const
        {
            return uint128_t(0, lo & rhs);
        }
        uint256_t operator&(const uint256_t& rhs) const
        {
            return uint256_t(hi & rhs.hi, lo & rhs.lo);
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        uint256_t operator&(const T& rhs) const
        {
            return uint256_t(static_cast<uint128_t>(0), lo & (uint128_t) rhs);
        }

        uint256_t& operator&=(const uint128_t& rhs)
        {
            hi = 0;
            lo &= rhs;
            return *this;
        }
        uint256_t& operator&=(const uint256_t& rhs)
        {
            hi &= rhs.hi;
            lo &= rhs.lo;
            return *this;
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        uint256_t& operator&=(const T& rhs)
        {
            hi = uint128_0;
            lo &= rhs;
            return *this;
        }

        uint256_t operator|(const uint128_t& rhs) const
        {
            return uint256_t(hi, lo | rhs);
        }
        uint256_t operator|(const uint256_t& rhs) const
        {
            return uint256_t(hi | rhs.hi, lo | rhs.lo);
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        uint256_t operator|(const T& rhs) const
        {
            return uint256_t(hi, lo | uint128_t(rhs));
        }

        uint256_t& operator|=(const uint128_t& rhs)
        {
            lo |= rhs;
            return *this;
        }
        uint256_t& operator|=(const uint256_t& rhs)
        {
            hi |= rhs.hi;
            lo |= rhs.lo;
            return *this;
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        uint256_t& operator|=(const T& rhs)
        {
            lo |= (uint128_t) rhs;
            return *this;
        }

        uint256_t operator^(const uint128_t& rhs) const
        {
            return uint256_t(hi, lo ^ rhs);
        }
        uint256_t operator^(const uint256_t& rhs) const
        {
            return uint256_t(hi ^ rhs.hi, lo ^ rhs.lo);
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        uint256_t operator^(const T& rhs) const
        {
            return uint256_t(hi, lo ^ (uint128_t) rhs);
        }

        uint256_t& operator^=(const uint128_t& rhs)
        {
            lo ^= rhs;
            return *this;
        }
        uint256_t& operator^=(const uint256_t& rhs)
        {
            hi ^= rhs.hi;
            lo ^= rhs.lo;
            return *this;
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        uint256_t& operator^=(const T& rhs)
        {
            lo ^= (uint128_t) rhs;
            return *this;
        }

        uint256_t operator~() const
        {
            return uint256_t(~hi, ~lo);
        }

        // Bit Shift Operators
        uint256_t operator<<(const uint128_t& shift) const
        {
            return *this << uint256_t(shift);
        }
        uint256_t operator<<(const uint256_t& shift) const;

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        uint256_t operator<<(const T& rhs) const
        {
            return *this << uint256_t(rhs);
        }

        uint256_t& operator<<=(const uint128_t& shift)
        {
            return *this <<= uint256_t(shift);
        }
        uint256_t& operator<<=(const uint256_t& shift)
        {
            *this = *this << shift;
            return *this;
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        uint256_t& operator<<=(const T& rhs)
        {
            *this = *this << uint256_t(rhs);
            return *this;
        }

        uint256_t operator>>(const uint128_t& shift) const
        {
            return *this >> uint256_t(shift);
        }
        uint256_t operator>>(const uint256_t& shift) const;

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        uint256_t operator>>(const T& rhs) const
        {
            return *this >> uint256_t(rhs);
        }

        uint256_t& operator>>=(const uint128_t& shift)
        {
            return *this >>= uint256_t(shift);
        }
        uint256_t& operator>>=(const uint256_t& shift)
        {
            *this = *this >> shift;
            return *this;
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        uint256_t& operator>>=(const T& rhs)
        {
            *this = *this >> uint256_t(rhs);
            return *this;
        }

        // Logical Operators
        bool operator!() const
        {
            return !static_cast<bool>(*this);
        }

        bool operator&&(const uint128_t& rhs) const
        {
            return (*this && uint256_t(rhs));
        }
        bool operator&&(const uint256_t& rhs) const
        {
            return (static_cast<bool>(*this) && static_cast<bool>(rhs));
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        bool operator&&(const T& rhs) const
        {
            return ((bool) *this && rhs);
        }

        bool operator||(const uint128_t& rhs) const
        {
            return (*this || uint256_t(rhs));
        }
        bool operator||(const uint256_t& rhs) const
        {
            return (static_cast<bool>(*this) || static_cast<bool>(rhs));
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        bool operator||(const T& rhs) const
        {
            return ((bool) *this || rhs);
        }

        // Comparison Operators
        bool operator==(const uint128_t& rhs) const
        {
            return (*this == uint256_t(rhs));
        }
        bool operator==(const uint256_t& rhs) const
        {
            return ((hi == rhs.hi) && (lo == rhs.lo));
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        bool operator==(const T& rhs) const
        {
            return (!hi && (lo == uint128_t(rhs)));
        }

        bool operator!=(const uint128_t& rhs) const
        {
            return (*this != uint256_t(rhs));
        }
        bool operator!=(const uint256_t& rhs) const
        {
            return ((hi != rhs.hi) | (lo != rhs.lo));
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        bool operator!=(const T& rhs) const
        {
            return ((bool) hi | (lo != uint128_t(rhs)));
        }

        bool operator>(const uint128_t& rhs) const
        {
            return (*this > uint256_t(rhs));
        }
        bool operator>(const uint256_t& rhs) const
        {
            if (hi == rhs.hi)
                return (lo > rhs.lo);
            return (hi > rhs.hi);
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        bool operator>(const T& rhs) const
        {
            return ((bool) hi | (lo > uint128_t(rhs)));
        }

        bool operator<(const uint128_t& rhs) const
        {
            return (*this < uint256_t(rhs));
        }
        bool operator<(const uint256_t& rhs) const
        {
            if (hi == rhs.hi)
                return lo < rhs.lo;
            return hi < rhs.hi;
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        bool operator<(const T& rhs) const
        {
            return (!hi) ? (lo < uint128_t(rhs)) : false;
        }

        bool operator>=(const uint128_t& rhs) const
        {
            return (*this >= uint256_t(rhs));
        }
        bool operator>=(const uint256_t& rhs) const
        {
            return ((*this > rhs) | (*this == rhs));
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        bool operator>=(const T& rhs) const
        {
            return ((*this > rhs) | (*this == rhs));
        }

        bool operator<=(const uint128_t& rhs) const
        {
            return (*this <= uint256_t(rhs));
        }
        bool operator<=(const uint256_t& rhs) const
        {
            return ((*this < rhs) | (*this == rhs));
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        bool operator<=(const T& rhs) const
        {
            return ((*this < rhs) | (*this == rhs));
        }

        // Arithmetic Operators
        uint256_t operator+(const uint128_t& rhs) const
        {
            return *this + uint256_t(rhs);
        }
        uint256_t operator+(const uint256_t& rhs) const
        {
            return uint256_t(hi + rhs.hi + (((lo + rhs.lo) < lo) ? 1 : 0), lo + rhs.lo);
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        uint256_t operator+(const T& rhs) const
        {
            return uint256_t(hi + ((lo + (uint128_t) rhs) < lo), lo + (uint128_t) rhs);
        }

        uint256_t& operator+=(const uint128_t& rhs)
        {
            return *this += uint256_t(rhs);
        }
        uint256_t& operator+=(const uint256_t& rhs)
        {
            hi = rhs.hi + hi + ((lo + rhs.lo) < lo);
            lo = lo + rhs.lo;
            return *this;
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        uint256_t& operator+=(const T& rhs)
        {
            hi = hi + ((lo + rhs) < lo);
            lo = lo + rhs;
            return *this;
        }

        uint256_t operator-(const uint128_t& rhs) const
        {
            return *this - uint256_t(rhs);
        }
        uint256_t operator-(const uint256_t& rhs) const
        {
            return uint256_t(hi - rhs.hi - ((lo - rhs.lo) > lo), lo - rhs.lo);
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        uint256_t operator-(const T& rhs) const
        {
            return uint256_t(hi - ((lo - rhs) > lo), lo - rhs);
        }

        uint256_t& operator-=(const uint128_t& rhs)
        {
            return *this -= uint256_t(rhs);
        }
        uint256_t& operator-=(const uint256_t& rhs)
        {
            *this = *this - rhs;
            return *this;
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        uint256_t& operator-=(const T& rhs)
        {
            *this = *this - rhs;
            return *this;
        }

        uint256_t operator*(const uint128_t& rhs) const
        {
            return *this * uint256_t(rhs);
        }
        uint256_t operator*(const uint256_t& rhs) const;

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        uint256_t operator*(const T& rhs) const
        {
            return *this * uint256_t(rhs);
        }

        uint256_t& operator*=(const uint128_t& rhs)
        {
            return *this *= uint256_t(rhs);
        }
        uint256_t& operator*=(const uint256_t& rhs)
        {
            *this = *this * rhs;
            return *this;
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        uint256_t& operator*=(const T& rhs)
        {
            *this = *this * uint256_t(rhs);
            return *this;
        }

        uint256_t operator/(const uint128_t& rhs) const
        {
            return *this / uint256_t(rhs);
        }
        uint256_t operator/(const uint256_t& rhs) const
        {
            return divmod(*this, rhs).first;
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        uint256_t operator/(const T& rhs) const
        {
            return *this / uint256_t(rhs);
        }

        uint256_t& operator/=(const uint128_t& rhs)
        {
            return *this /= uint256_t(rhs);
        }
        uint256_t& operator/=(const uint256_t& rhs)
        {
            *this = *this / rhs;
            return *this;
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        uint256_t& operator/=(const T& rhs)
        {
            *this = *this / uint256_t(rhs);
            return *this;
        }

        uint256_t operator%(const uint128_t& rhs) const
        {
            return *this % uint256_t(rhs);
        }
        uint256_t operator%(const uint256_t& rhs) const
        {
            return *this - (rhs * (*this / rhs));
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        uint256_t operator%(const T& rhs) const
        {
            return *this % uint256_t(rhs);
        }

        uint256_t& operator%=(const uint128_t& rhs)
        {
            return *this %= uint256_t(rhs);
        }
        uint256_t& operator%=(const uint256_t& rhs)
        {
            *this = *this % rhs;
            return *this;
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
        uint256_t& operator%=(const T& rhs)
        {
            *this = *this % uint256_t(rhs);
            return *this;
        }

        // Increment Operators
        uint256_t& operator++()
        {
            *this += 1;
            return *this;
        }
        uint256_t operator++(int)
        {
            uint256_t temp(*this);
            ++*this;
            return temp;
        }

        // Decrement Operators
        uint256_t& operator--()
        {
            *this -= 1;
            return *this;
        }
        uint256_t operator--(int)
        {
            uint256_t temp(*this);
            --*this;
            return temp;
        }

        // Nothing done since promotion doesn't work here
        uint256_t operator+() const
        {
            return *this;
        }

        // two's complement
        uint256_t operator-() const
        {
            return ~*this + 1;
        }

        // Get bitsize of value
        uint16_t bits() const;

        // Get string representation of value
        std::string str(uint8_t base = 10, const unsigned int& len = 0) const;
    };

    // Bitwise Operators
    inline uint256_t operator&(const uint128_t& lhs, const uint256_t& rhs)
    {
        return rhs & lhs;
    }

    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    uint256_t operator&(const T& lhs, const uint256_t& rhs)
    {
        return rhs & lhs;
    }

    inline uint128_t& operator&=(uint128_t& lhs, const uint256_t& rhs)
    {
        lhs = (rhs & lhs).lo;
        return lhs;
    }

    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    T& operator&=(T& lhs, const uint256_t& rhs)
    {
        return lhs = static_cast<T> (rhs & lhs);
    }

    inline uint256_t operator|(const uint128_t& lhs, const uint256_t& rhs)
    {
        return rhs | lhs;
    }

    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    uint256_t operator|(const T& lhs, const uint256_t& rhs)
    {
        return rhs | lhs;
    }

    inline uint128_t& operator|=(uint128_t& lhs, const uint256_t& rhs)
    {
        lhs = (rhs | lhs).lo;
        return lhs;
    }

    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    T& operator|=(T& lhs, const uint256_t& rhs)
    {
        return lhs = static_cast<T> (rhs | lhs);
    }

    inline uint256_t operator^(const uint128_t& lhs, const uint256_t& rhs)
    {
        return rhs ^ lhs;
    }

    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    uint256_t operator^(const T& lhs, const uint256_t& rhs)
    {
        return rhs ^ lhs;
    }

    inline uint128_t& operator^=(uint128_t& lhs, const uint256_t& rhs)
    {
        lhs = (rhs ^ lhs).lo;
        return lhs;
    }

    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    T& operator^=(T& lhs, const uint256_t& rhs)
    {
        return lhs = static_cast<T> (rhs ^ lhs);
    }

    // Bitshift operators
    inline uint256_t operator<<(const bool      & lhs, const uint256_t& rhs) { return uint256_t(lhs) << rhs; }
    inline uint256_t operator<<(const uint8_t   & lhs, const uint256_t& rhs) { return uint256_t(lhs) << rhs; }
    inline uint256_t operator<<(const uint16_t  & lhs, const uint256_t& rhs) { return uint256_t(lhs) << rhs; }
    inline uint256_t operator<<(const uint32_t  & lhs, const uint256_t& rhs) { return uint256_t(lhs) << rhs; }
    inline uint256_t operator<<(const uint64_t  & lhs, const uint256_t& rhs) { return uint256_t(lhs) << rhs; }
    inline uint256_t operator<<(const uint128_t & lhs, const uint256_t& rhs) { return uint256_t(lhs) << rhs; }
    inline uint256_t operator<<(const int8_t    & lhs, const uint256_t& rhs) { return uint256_t(lhs) << rhs; }
    inline uint256_t operator<<(const int16_t   & lhs, const uint256_t& rhs) { return uint256_t(lhs) << rhs; }
    inline uint256_t operator<<(const int32_t   & lhs, const uint256_t& rhs) { return uint256_t(lhs) << rhs; }
    inline uint256_t operator<<(const int64_t   & lhs, const uint256_t& rhs) { return uint256_t(lhs) << rhs; }

    inline uint128_t& operator<<=(uint128_t& lhs, const uint256_t& rhs)
    {
        lhs = (uint256_t(lhs) << rhs).lo;
        return lhs;
    }

    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    T& operator<<=(T& lhs, const uint256_t& rhs)
    {
        lhs = static_cast<T> (uint256_t(lhs) << rhs);
        return lhs;
    }

    inline uint256_t operator>>(const bool      & lhs, const uint256_t& rhs) { return uint256_t(lhs) >> rhs; }
    inline uint256_t operator>>(const uint8_t   & lhs, const uint256_t& rhs) { return uint256_t(lhs) >> rhs; }
    inline uint256_t operator>>(const uint16_t  & lhs, const uint256_t& rhs) { return uint256_t(lhs) >> rhs; }
    inline uint256_t operator>>(const uint32_t  & lhs, const uint256_t& rhs) { return uint256_t(lhs) >> rhs; }
    inline uint256_t operator>>(const uint64_t  & lhs, const uint256_t& rhs) { return uint256_t(lhs) >> rhs; }
    inline uint256_t operator>>(const uint128_t & lhs, const uint256_t& rhs) { return uint256_t(lhs) >> rhs; }
    inline uint256_t operator>>(const int8_t    & lhs, const uint256_t& rhs) { return uint256_t(lhs) >> rhs; }
    inline uint256_t operator>>(const int16_t   & lhs, const uint256_t& rhs) { return uint256_t(lhs) >> rhs; }
    inline uint256_t operator>>(const int32_t   & lhs, const uint256_t& rhs) { return uint256_t(lhs) >> rhs; }
    inline uint256_t operator>>(const int64_t   & lhs, const uint256_t& rhs) { return uint256_t(lhs) >> rhs; }

    inline uint128_t& operator>>=(uint128_t& lhs, const uint256_t& rhs)
    {
        lhs = (uint256_t(lhs) >> rhs).lo;
        return lhs;
    }

    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    T& operator>>=(T& lhs, const uint256_t& rhs)
    {
        return lhs = static_cast<T> (uint256_t(lhs) >> rhs);
    }

    // Comparison Operators
    inline bool operator==(const uint128_t& lhs, const uint256_t& rhs)
    {
        return rhs == lhs;
    }

    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    bool operator==(const T& lhs, const uint256_t& rhs)
    {
        return (!rhs.hi && ((uint64_t) lhs == rhs.lo));
    }

    inline bool operator!=(const uint128_t& lhs, const uint256_t& rhs)
    {
        return rhs != lhs;
    }

    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    bool operator!=(const T& lhs, const uint256_t& rhs)
    {
        return (rhs.hi | ((uint64_t) lhs != rhs.lo));
    }

    inline bool operator>(const uint128_t& lhs, const uint256_t& rhs)
    {
        return rhs < lhs;
    }

    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    bool operator>(const T& lhs, const uint256_t& rhs)
    {
        return rhs.hi ? false : ((uint128_t) lhs > rhs.lo);
    }

    inline bool operator<(const uint128_t& lhs, const uint256_t& rhs)
    {
        return rhs > lhs;
    }

    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    bool operator<(const T& lhs, const uint256_t& rhs)
    {
        return rhs.hi ? true : ((uint128_t) lhs < rhs.lo);
    }

    inline bool operator>=(const uint128_t& lhs, const uint256_t& rhs)
    {
        return rhs <= lhs;
    }

    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    bool operator>=(const T& lhs, const uint256_t& rhs)
    {
        return rhs.hi?false:((uint128_t) lhs >= rhs.lo);
    }

    inline bool operator<=(const uint128_t& lhs, const uint256_t& rhs)
    {
        return rhs >= lhs;
    }

    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    bool operator<=(const T& lhs, const uint256_t& rhs)
    {
        return rhs.hi?true:((uint128_t) lhs <= rhs.lo);
    }

    // Arithmetic Operators
    inline uint256_t operator+(const uint128_t& lhs, const uint256_t& rhs)
    {
        return rhs + lhs;
    }

    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    uint256_t operator+(const T& lhs, const uint256_t& rhs)
    {
        return rhs + lhs;
    }

    inline uint128_t& operator+=(uint128_t& lhs, const uint256_t& rhs)
    {
        lhs = (rhs + lhs).lo;
        return lhs;
    }

    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    T& operator+=(T& lhs, const uint256_t& rhs)
    {
        lhs = static_cast<T> (rhs + lhs);
        return lhs;
    }

    inline uint256_t operator-(const uint128_t& lhs, const uint256_t& rhs)
    {
        return -(rhs - lhs);
    }

    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    uint256_t operator-(const T& lhs, const uint256_t& rhs)
    {
        return -(rhs - lhs);
    }

    inline uint128_t& operator-=(uint128_t& lhs, const uint256_t& rhs)
    {
        lhs = (-(rhs - lhs)).lo;
        return lhs;
    }

    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    T& operator-=(T& lhs, const uint256_t& rhs)
    {
        return lhs = static_cast<T> (-(rhs - lhs));
    }

    inline uint256_t operator*(const uint128_t& lhs, const uint256_t& rhs)
    {
        return rhs * lhs;
    }

    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    uint256_t operator*(const T& lhs, const uint256_t& rhs)
    {
        return rhs * lhs;
    }

    inline uint128_t& operator*=(uint128_t& lhs, const uint256_t& rhs)
    {
        lhs = (rhs * lhs).lo;
        return lhs;
    }

    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    T& operator*=(T& lhs, const uint256_t& rhs)
    {
        return lhs = static_cast<T> (rhs * lhs);
    }

    inline uint256_t operator/(const uint128_t& lhs, const uint256_t& rhs)
    {
        return uint256_t(lhs) / rhs;
    }

    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    uint256_t operator/(const T& lhs, const uint256_t& rhs)
    {
        return uint256_t(lhs) / rhs;
    }

    inline uint128_t& operator/=(uint128_t& lhs, const uint256_t& rhs)
    {
        lhs = (uint256_t(lhs) / rhs).lo;
        return lhs;
    }

    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    T& operator/=(T& lhs, const uint256_t& rhs)
    {
        return lhs = static_cast<T> (uint256_t(lhs) / rhs);
    }

    inline uint256_t operator%(const uint128_t& lhs, const uint256_t& rhs)
    {
        return uint256_t(lhs) % rhs;
    }

    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    uint256_t operator%(const T& lhs, const uint256_t& rhs)
    {
        return uint256_t(lhs) % rhs;
    }

    inline uint128_t& operator%=(uint128_t& lhs, const uint256_t& rhs)
    {
        lhs = (uint256_t(lhs) % rhs).lo;
        return lhs;
    }

    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    T& operator%=(T& lhs, const uint256_t& rhs)
    {
        return lhs = static_cast<T> (uint256_t(lhs) % rhs);
    }

    // IO Operator
    inline std::ostream & operator<<(std::ostream & stream, const uint256_t& rhs)
    {
        if (stream.flags() & stream.oct)
            stream << rhs.str(8);
        else if (stream.flags() & stream.dec)
            stream << rhs.str(10);
        else if (stream.flags() & stream.hex)
            stream << rhs.str(16);
        return stream;
    }

    // ────────────────────────────────────────────────────────────────────────────────

    inline uint256_t uint256_t::operator<<(const uint256_t& rhs) const
    {
        static const uint256_t u0(0);
        static const uint256_t u128(128);
        static const uint256_t u256(256);

        const uint128_t shift = rhs.lo;
        if (((bool) rhs.hi) || (shift >= u256))
            return u0;
        else if (shift == u128)
            return uint256_t(lo, u0);
        else if (shift == u0)
            return *this;
        else if (shift < u128)
            return uint256_t((hi << shift) + (lo >> (u128 - shift)), lo << shift);
        else if ((u256 > shift) && (shift > u128))
            return uint256_t(lo << (shift - u128), u0);
        else
            return u0;
    }

    inline uint256_t uint256_t::operator>>(const uint256_t& rhs) const
    {
        static const uint128_t u0(0);
        static const uint128_t u128(128);
        static const uint128_t u256(256);

        const uint128_t shift = rhs.lo;
        if (((bool) rhs.hi) | (shift >= u256))
            return u0;
        else if (shift == u128)
            return uint256_t(hi);
        else if (shift == u0)
            return *this;
        else if (shift < u128)
            return uint256_t(hi >> shift, (hi << (u128 - shift)) + (lo >> shift));
        else if ((u256 > shift) && (shift > u128))
            return uint256_t(hi >> (shift - u128));
        else
            return u0;
    }

    inline uint256_t uint256_t::operator*(const uint256_t & rhs) const
    {
        static const uint128_t u0(0);
        static const uint128_t u64(64);

        uint128_t top[4] = {hi.hi, hi.lo, lo.hi, lo.lo};
        uint128_t bottom[4] = {rhs.hi.hi, rhs.hi.lo, rhs.lo.hi, rhs.lo.lo};
        uint128_t products[4][4];

        // multiply each component of the values
        for(int y = 3; y > -1; y--)
            for(int x = 3; x > -1; x--)
                products[3 - y][x] = top[x] * bottom[y];

        // first row
        uint128_t fourth64 = uint128_t(products[0][3].lo);
        uint128_t third64  = uint128_t(products[0][2].lo) + uint128_t(products[0][3].hi);
        uint128_t second64 = uint128_t(products[0][1].lo) + uint128_t(products[0][2].hi);
        uint128_t first64  = uint128_t(products[0][0].lo) + uint128_t(products[0][1].hi);

        // second row
        third64  += uint128_t(products[1][3].lo);
        second64 += uint128_t(products[1][2].lo) + uint128_t(products[1][3].hi);
        first64  += uint128_t(products[1][1].lo) + uint128_t(products[1][2].hi);

        // third row
        second64 += uint128_t(products[2][3].lo);
        first64  += uint128_t(products[2][2].lo) + uint128_t(products[2][3].hi);

        // fourth row
        first64  += uint128_t(products[3][3].lo);

        // combines the values, taking care of carry over
        return uint256_t(first64 << u64, u0) +
            uint256_t(third64.hi, third64 << u64) +
            uint256_t(second64, u0) +
            uint256_t(fourth64);
    }

    inline std::pair <uint256_t, uint256_t> uint256_t::divmod(const uint256_t & lhs, const uint256_t & rhs) const
    {
        static const uint256_t u0(0);
        static const uint256_t u1(1);

        if (rhs == u0)
            throw std::domain_error("Error: division or modulus by 0");
        else if (rhs == u1)
            return std::pair <uint256_t, uint256_t> (lhs, u0);
        else if (lhs == rhs)
            return std::pair <uint256_t, uint256_t> (u1, u0);
        else if ((lhs == u0) || (lhs < rhs))
            return std::pair <uint256_t, uint256_t> (u0, lhs);

        std::pair <uint256_t, uint256_t> p(u0, lhs);
        uint256_t copyd = rhs << (lhs.bits() - rhs.bits());
        uint256_t adder = u1 << (lhs.bits() - rhs.bits());
        if (copyd > p.second)
        {
            copyd >>= u1;
            adder >>= u1;
        }

        while (p.second >= rhs)
        {
            if (p.second >= copyd)
            {
                p.second -= copyd;
                p.first |= adder;
            }
            copyd >>= u1;
            adder >>= u1;
        }
        return p;
    }
    inline uint16_t uint256_t::bits() const
    {
        static const uint128_t u1(1);
        uint16_t out = 0;
        if (hi)
        {
            out = 128;
            uint128_t up = hi;
            while (up)
            {
                up >>= u1;
                out++;
            }
        }
        else
        {
            uint128_t low = lo;
            while (low)
            {
                low >>= u1;
                out++;
            }
        }
        return out;
    }

    inline std::string uint256_t::str(uint8_t base, const unsigned int & len) const
    {
        static const uint256_t u0(0);

        if ((base < 2) || (base > 36))
            throw std::invalid_argument("Base must be in the range 2-36");

        std::string out = "";
        if (!(*this))
        {
            out = "0";
        }
        else
        {
            std::pair <uint256_t, uint256_t> p(*this, u0);
            do
            {
                p = divmod(p.first, base);
                out = "0123456789abcdefghijklmnoppstuvwxyz"[(uint8_t) p.second] + out;
            }
            while (p.first);
        }

        if (out.size() < len)
            out = std::string(len - out.size(), '0') + out;
        return out;
    }
} // namespace pride
