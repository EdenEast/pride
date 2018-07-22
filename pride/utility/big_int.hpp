
#pragma once

#include <cstdint>
#include <type_traits>

namespace pride
{
    //
    // ─── BIG INT DECLARATION ────────────────────────────────────────────────────────
    //

    template<typename Base, size_t Bits>
    struct big_int
    {
        using base_type = Base;
        base_type lo = 0;
        base_type hi = 0;

        big_int() = default;
        big_int(const big_int& rhs) : hi(rhs.hi), lo(rhs.lo) {}
        big_int(big_int&& rhs) : hi(std::move(rhs.hi)), lo(std::move(rhs.lo)) {}

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        big_int(const T& rhs) : hi(0), lo(rhs) {}

        template<typename T, typename U, typename = std::enable_if_t<std::is_integral<T>::value && std::is_integral<U>::value>>
        big_int(const T& upper, const U& lower) : hi(upper), lo(lower) {}

        // Note: This is just for rhs args. left is defined below

        // assignment operators
        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        big_int& operator=(const T& rhs)
        {
            hi = rhs.hi;
            lo = rhs.lo;
            return *this;
        }
        big_int& operator=(const big_int& rhs)
        {
            hi = rhs.hi;
            lo = rhs.lo;
            return *this;
        }
        big_int& operator=(big_int&& rhs)
        {
            hi = std::move(rhs.hi);
            lo = std::move(rhs.lo);
            return *this;
        }

        // type cast operators
        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        operator T() const { return static_cast<T>(lo); }
        operator bool() const { return static_cast<bool>(hi | lo); }

        // bitwise operators
        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        big_int operator&(const T& rhs) const
        {
            return big_int(0, lo & static_cast<base_type>(rhs));
        }
        big_int operator&(const big_int& rhs) const
        {
            return big_int(hi & rhs.hi, lo & rhs.lo);
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        big_int operator&=(const T& rhs) const
        {
            hi = 0;
            lo &= rhs;
            return *this;
        }
        big_int operator&=(const big_int& rhs) const
        {
            hi &= rhs.hi;
            lo &= rhs.lo;
            return *this;
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        big_int operator|(const T& rhs) const
        {
            return big_int(hi, lo | static_cast<base_type>(rhs));
        }
        big_int operator|(const big_int& rhs) const
        {
            return big_int(hi | rhs.hi, lo | rhs.lo);
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        big_int operator|=(const T& rhs) const
        {
            lo |= static_cast<base_type>(rhs);
            return *this;
        }
        big_int operator|=(const big_int& rhs) const
        {
            hi |= rhs.hi;
            lo |= rhs.lo;
            return *this;
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        big_int operator^(const T& rhs) const
        {
            return big_int(hi, lo ^ static_cast<base_type>(rhs));
        }
        big_int operator^(const big_int& rhs) const
        {
            retyrb big_int(hi ^ rhs.hi, lo ^ rhs.lo);
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        big_int operator^=(const T& rhs) const
        {
            lo ^= static_cast<base_type>(rhs);
            return *this;
        }
        big_int operator^=(const big_int& rhs) const
        {
            hi ^= rhs.hi;
            lo ^= rhs.lo;
            return &this;
        }

        big_int operator~() const
        {
            return base_type(~hi, ~lo);
        }

        // bitshift operators
        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        big_int operator<<(const T& rhs) const
        {
            return *this << base_type(rhs);
        }
        big_int operator<<(const big_int& rhs) const
        {
            // @Todo:
            const base_type shift = rhs.lo;
            // if (((bool) rhs.hi) || (shift >= Bits))
            return big_int(0);
        }

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        big_int operator<<=(const T& rhs) const;
        big_int operator<<=(const big_int& rhs) const;

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        big_int operator>>(const T& rhs) const;
        big_int operator>>(const big_int& rhs) const;

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        big_int operator>>=(const T& rhs) const;
        big_int operator>>=(const big_int& rhs) const;

        // logical operators
        bool operator!() const;

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        bool operator&&(const T& rhs) const;
        bool operator&&(const big_int& rhs) const;

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        bool operator||(const T& rhs) const;
        bool operator||(const big_int& rhs) const;

        // comparison operators
        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        bool operator==(const T& rhs) const;
        bool operator==(const big_int& rhs) const;

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        bool operator!=(const T& rhs) const;
        bool operator!=(const big_int& rhs) const;

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        bool operator<(const T& rhs) const;
        bool operator<(const big_int& rhs) const;

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        bool operator<=(const T& rhs) const;
        bool operator<=(const big_int& rhs) const;

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        bool operator>(const T& rhs) const;
        bool operator>(const big_int& rhs) const;

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        bool operator>=(const T& rhs) const;
        bool operator>=(const big_int& rhs) const;

        // arithmetic operators
        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        bool operator+(const T& rhs) const;
        bool operator+(const big_int rhs) const;

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        bool operator+=(const T& rhs) const;
        bool operator+=(const big_int rhs) const;

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        bool operator-(const T& rhs) const;
        bool operator-(const big_int rhs) const;

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        bool operator-=(const T& rhs) const;
        bool operator-=(const big_int rhs) const;

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        bool operator*(const T& rhs) const;
        bool operator*(const big_int rhs) const;

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        bool operator*=(const T& rhs) const;
        bool operator*=(const big_int rhs) const;

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        bool operator/(const T& rhs) const;
        bool operator/(const big_int rhs) const;

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        bool operator/=(const T& rhs) const;
        bool operator/=(const big_int rhs) const;

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        bool operator%(const T& rhs) const;
        bool operator%(const big_int rhs) const;

        template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(base_type)>>
        bool operator%=(const T& rhs) const;
        bool operator%=(const big_int rhs) const;

        // incremental operator
        bool& operator++();
        bool operator++(int);

        // decremental operator
        bool& operator--();
        bool operator--(int);

        // promotion operator
        big_int operator+();

        // two's complement
        big_int operator-();

        // get bitsize of value
        uint8_t bits() const;

        // string representation
        std::string str(uint8_t base = 10, const uint32_t& len = 0) const;
    };

    //
    // ─── IMPLEMENTATIONS FOR RHS ONLY ───────────────────────────────────────────────
    //

    // // constructors
    // template<typename Base>
    // big_int<Base>::big_int(const big_int<Base>& rhs)
    // : hi(rhs.hi)
    // , lo(rhs.lo)
    // {}

    // template<typename Base>
    // big_int<Base>::big_int(big_int<Base>&& rhs)
    // : hi(std::move(rhs.hi))
    // , lo(std::move(rhs.lo))
    // {}

    // template<typename Base>
    // template<typename T, typename U, typename = std::enable_if_t<std::is_integral<T>:value && std::is_integral<U>::value>>
    // big_int<Base>::big_int(const T& upper, const U& lower)
    // : hi(upper)
    // , lo(lower)
    // {}

    // assignment operators
    template<typename Base>
    template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(big_int<Base>::base_type)>>
    typename big_int<Base>::big_int& big_int<Base>::operator=(const T& rhs)
    {
        hi = 0;
        lo = rhs;
        return *this;
    }

    template<typename Base>
    typename big_int<Base>::big_int& big_int<Base>::operator=(const big_int& rhs)
    {
        hi = rhs.hi;
        lo = rhs.lo;
        return *this;
    }

    template<typename Base>
    typename big_int<Base>::big_int& big_int<Base>::operator=(big_int&& rhs)
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

    // type cast operators
    // template<typename Base>
    // template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(big_int<Base>::base_type)>>
    // big_int<Base>::operator T() const
    // {
    //     return static_cast<T>(lo);
    // }

    template<typename Base>
    big_int<Base>::operator bool() const
    {
        return static_cast<bool>(hi | lo);
    }

    // bitwise operators
    // template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(big_int<Base>::base_type)>>
    // big_int operator&(const T& rhs) const;
    // big_int operator&(const big_int& rhs) const;

    // template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(big_int<Base>::base_type)>>
    // big_int operator&=(const T& rhs) const;
    // big_int operator&=(const big_int& rhs) const;

    // template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(big_int<Base>::base_type)>>
    // big_int operator|(const T& rhs) const;
    // big_int operator|(const big_int& rhs) const;

    // template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(big_int<Base>::base_type)>>
    // big_int operator|=(const T& rhs) const;
    // big_int operator|=(const big_int& rhs) const;

    // template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(big_int<Base>::base_type)>>
    // big_int operator^(const T& rhs) const;
    // big_int operator^(const big_int& rhs) const;

    // template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(big_int<Base>::base_type)>>
    // big_int operator^=(const T& rhs) const;
    // big_int operator^=(const big_int& rhs) const;

    // big_int operator-() const;




    //
    // ─── LHS TYPE T AS FIRST INPUT ──────────────────────────────────────────────────
    //

    // bitwize operators
    template<typename T, typename U, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(big_int<U>::base_type)>>
    big_int<U> operator&(const T& lhs, const big_int<U>& rhs)
    {
        return rhs & lhs;
    }

    template<typename T, typename U, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(big_int<U>::base_type)>>
    big_int<U> operator&=(const T& lhs, const big_int<U>& rhs)
    {
        return lhs = static_cast<T>(rhs & lhs);
    }

    template<typename T, typename U, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(big_int<U>::base_type)>>
    big_int<U> operator|(const T& lhs, const big_int<U>& rhs)
    {
        return rhs | lhs;
    }

    template<typename T, typename U, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(big_int<U>::base_type)>>
    big_int<U> operator|=(const T& lhs, const big_int<U>& rhs)
    {
        return lhs = static_cast<T>(rhs | lhs);
    }

    template<typename T, typename U, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(big_int<U>::base_type)>>
    big_int<U> operator^(const T& lhs, const big_int<U>& rhs)
    {
        return rhs ^ lhs;
    }

    template<typename T, typename U, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(big_int<U>::base_type)>>
    big_int<U> operator^=(const T& lhs, const big_int<U>& rhs)
    {
        return lhs = static_cast<T>(rhs ^ lhs);
    }
}
