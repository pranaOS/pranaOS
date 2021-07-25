/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Checked.h>
#include <base/Concepts.h>
#include <base/Format.h>
#include <base/NumericLimits.h>
#include <base/StdLibExtraDetails.h>
#include <base/StdLibExtras.h>
#include <base/String.h>
#include <base/StringBuilder.h>

namespace Base {

template<typename T>
requires(sizeof(T) >= sizeof(u64) && IsUnsigned<T>) class UFixedBigInt;

template<typename T>
constexpr inline bool Detail::IsUnsigned<UFixedBigInt<T>> = true;
template<typename T>
constexpr inline bool Detail::IsSigned<UFixedBigInt<T>> = false;

template<typename T>
struct NumericLimits<UFixedBigInt<T>> {
    static constexpr UFixedBigInt<T> min() { return 0; }
    static constexpr UFixedBigInt<T> max() { return { NumericLimits<T>::max(), NumericLimits<T>::max() }; }
    static constexpr bool is_signed() { return false; }
};


template<typename T>
requires(sizeof(T) >= sizeof(u64) && IsUnsigned<T>) class UFixedBigInt {
public:
    using R = UFixedBigInt<T>;
    
    constexpr UFixedBigInt() = default;
    template<Unsigned U>
    requires(sizeof(T) >= sizeof(U)) constexpr UFixedBigInt(U low)
        : m_low(low)
        , m_high(0u)
    {
    }

    constexpr T& low()
    {
        return m_low;
    }

    constexpr const T& low() const
    {
        return m_low;
    }

    constexpr T& high()
    {
        return m_high;
    }

    constexpr const T& high() const
    {
        return m_high;
    }

        Span<u8> bytes()
    {
        return Span<u8>(reinterpret_cast<u8*>(this), sizeof(R));
    }
    Span<const u8> bytes() const
    {
        return Span<const u8>(reinterpret_cast<const u8*>(this), sizeof(R));
    }

    template<Unsigned U>
    requires(sizeof(T) >= sizeof(U)) explicit operator U() const
    {
        return static_cast<U>(m_low);
    }

    constexpr size_t clz() const requires(IsSame<T, u64>)
    {
        if (m_high)
            return __builtin_clzll(m_high);
        else
            return sizeof(T) * 8 + __builtin_clzll(m_low);
    }
    constexpr size_t clz() const requires(!IsSame<T, u64>)
    {
        if (m_high)
            return m_high.clz();
        else
            return sizeof(T) * 8 + m_low.clz();
    }
    constexpr size_t ctz() const requires(IsSame<T, u64>)
    {
        if (m_low)
            return __builtin_ctzll(m_low);
        else
            return sizeof(T) * 8 + __builtin_ctzll(m_high);
    }
    constexpr size_t ctz() const requires(!IsSame<T, u64>)
    {
        if (m_low)
            return m_low.ctz();
        else
            return sizeof(T) * 8 + m_high.ctz();
    }

        constexpr size_t popcnt() const requires(IsSame<T, u64>)
    {
        return __builtin_popcntll(m_low) + __builtin_popcntll(m_high);
    }
    constexpr size_t popcnt() const requires(!IsSame<T, u64>)
    {
        return m_low.popcnt() + m_high.popcnt();
    }

    constexpr bool operator!() const
    {
        return !m_low && !m_high;
    }
    constexpr explicit operator bool() const
    {
        return m_low || m_high;
    }
    template<Unsigned U>
    requires(sizeof(T) >= sizeof(U)) constexpr bool operator==(const T& other) const
    {
        return !m_high && m_low == other;
    }

        template<Unsigned U>
    requires(sizeof(T) >= sizeof(U)) constexpr bool operator!=(const T& other) const
    {
        return m_high || m_low != other;
    }
    template<Unsigned U>
    requires(sizeof(T) >= sizeof(U)) constexpr bool operator>(const T& other) const
    {
        return m_high || m_low > other;
    }
    template<Unsigned U>
    requires(sizeof(T) >= sizeof(U)) constexpr bool operator<(const T& other) const
    {
        return !m_high && m_low < other;
    }
    template<Unsigned U>
    requires(sizeof(T) >= sizeof(U)) constexpr bool operator>=(const T& other) const
    {
        return *this == other || *this > other;
    }
    template<Unsigned U>
    requires(sizeof(T) >= sizeof(U)) constexpr bool operator<=(const T& other) const
    {
        return *this == other || *this < other;
    }

        constexpr bool operator==(const R& other) const
    {
        return m_low == other.low() && m_high == other.high();
    }
    constexpr bool operator!=(const R& other) const
    {
        return m_low != other.low() || m_high != other.high();
    }
    constexpr bool operator>(const R& other) const
    {
        return m_high > other.high()
            || (m_high == other.high() && m_low > other.low());
    }
    constexpr bool operator<(const R& other) const
    {
        return m_high < other.high()
            || (m_high == other.high() && m_low < other.low());
    }
    constexpr bool operator>=(const R& other) const
    {
        return *this == other || *this > other;
    }
    constexpr bool operator<=(const R& other) const
    {
        return *this == other || *this < other;
    }

    constexpr R operator~() const
    {
        return { ~m_low, ~m_high };
    }
    template<Unsigned U>
    requires(sizeof(T) >= sizeof(U)) constexpr U operator&(const U& other) const
    {
        return static_cast<const U>(m_low) & other;
    }
    template<Unsigned U>
    requires(sizeof(T) >= sizeof(U)) constexpr R operator|(const U& other) const
    {
        return { m_low | other, m_high };
    }
    template<Unsigned U>
    requires(sizeof(T) >= sizeof(U)) constexpr R operator^(const U& other) const
    {
        return { m_low ^ other, m_high };
    }
    template<Unsigned U>
    constexpr R operator<<(const U& shift) const
    {
        if (shift >= sizeof(R) * 8u)
            return 0u;
        if (shift >= sizeof(T) * 8u)
            return R { 0u, m_low << (shift - sizeof(T) * 8u) };
        if (!shift)
            return *this;

        T overflow = m_low >> (sizeof(T) * 8u - shift);
        return R { m_low << shift, (m_high << shift) | overflow };
    }
    template<Unsigned U>
    constexpr R operator>>(const U& shift) const
    {
        if (shift >= sizeof(R) * 8u)
            return 0u;
        if (shift >= sizeof(T) * 8u)
            return m_high >> (shift - sizeof(T) * 8u);
        if (!shift)
            return *this;

        T underflow = m_high << (sizeof(T) * 8u - shift);
        return R { (m_low >> shift) | underflow, m_high >> shift };
    }
    template<Unsigned U>
    constexpr R rol(const U& shift) const
    {
        return (*this >> sizeof(T) * 8u - shift) | (*this << shift);
    }
    template<Unsigned U>
    constexpr R ror(const U& shift) const
    {
        return (*this << sizeof(T) * 8u - shift) | (*this >> shift);
    }

    constexpr R operator&(const R& other) const
    {
        return { m_low & other.low(), m_high & other.high() };
    }
    constexpr R operator|(const R& other) const
    {
        return { m_low | other.low(), m_high | other.high() };
    }
    constexpr R operator^(const R& other) const
    {
        return { m_low ^ other.low(), m_high ^ other.high() };
    }

    template<Unsigned U>
    requires(sizeof(T) >= sizeof(U)) constexpr R& operator&=(const U& other)
    {
        m_high = 0u;
        m_low &= other;
        return *this;
    }
    template<Unsigned U>
    requires(sizeof(T) >= sizeof(U)) constexpr R& operator|=(const U& other)
    {
        m_low |= other;
        return *this;
    }
    template<Unsigned U>
    requires(sizeof(T) >= sizeof(U)) constexpr R& operator^=(const U& other)
    {
        m_low ^= other;
        return *this;
    }
    template<Unsigned U>
    constexpr R& operator>>=(const U& other)
    {
        *this = *this >> other;
        return *this;
    }
    template<Unsigned U>
    constexpr R& operator<<=(const U& other)
    {
        *this = *this << other;
        return *this;
    }

    constexpr R& operator&=(const R& other)
    {
        m_high &= other.high();
        m_low &= other.low();
        return *this;
    }
    constexpr R& operator|=(const R& other)
    {
        m_high |= other.high();
        m_low |= other.low();
        return *this;
    }
    constexpr R& operator^=(const R& other)
    {
        m_high ^= other.high();
        m_low ^= other.low();
        return *this;
    }

    static constexpr size_t my_size()
    {
        return sizeof(R);
    }


    template<Unsigned U>
    requires(sizeof(T) >= sizeof(U) && IsSame<T, u64>) constexpr R addc(const U other, bool& carry) const
    {
        bool low_carry = Checked<T>::addition_would_overflow(m_low, other);
        low_carry |= Checked<T>::addition_would_overflow(m_low, carry);
        bool high_carry = Checked<T>::addition_would_overflow(m_high, low_carry);

        T lower = m_low + other + carry;
        T higher = m_high + low_carry;

        carry = high_carry;

        return {
            lower,
            higher
        };
    }
    template<Unsigned U>
    requires(my_size() > sizeof(U) && sizeof(T) > sizeof(u64)) constexpr R addc(const U& other, bool& carry) const
    {
        T lower = m_low.addc(other, carry);
        T higher = m_high.addc(0u, carry);

        return {
            lower,
            higher
        };
    }

        template<Unsigned U>
    requires(IsSame<R, U>&& IsSame<T, u64>) constexpr R addc(const U& other, bool& carry) const
    {
        bool low_carry = Checked<T>::addition_would_overflow(m_low, other.low());
        bool high_carry = Checked<T>::addition_would_overflow(m_high, other.high());

        T lower = m_low + other.low();
        T higher = m_high + other.high();
        low_carry |= Checked<T>::addition_would_overflow(lower, carry);
        high_carry |= Checked<T>::addition_would_overflow(higher, low_carry);

        lower += carry;
        higher += low_carry;
        carry = high_carry;

        return {
            lower,
            higher
        };
    }
    template<Unsigned U>
    requires(IsSame<R, U> && sizeof(T) > sizeof(u64)) constexpr R addc(const U& other, bool& carry) const
    {
        T lower = m_low.addc(other.low(), carry);
        T higher = m_high.addc(other.high(), carry);

        return {
            lower,
            higher
        };
    }
    template<Unsigned U>
    requires(my_size() < sizeof(U)) constexpr U addc(const U& other, bool& carry) const
    {
        return other.addc(*this, carry);
    }

    template<Unsigned U>
    requires(sizeof(T) >= sizeof(U)) constexpr R subc(const U& other, bool& carry) const
    {
        bool low_carry = (!m_low && carry) || (m_low - carry) < other;
        bool high_carry = !m_high && low_carry;

        T lower = m_low - other - carry;
        T higher = m_high - low_carry;
        carry = high_carry;

        return { lower, higher };
    }
    constexpr R subc(const R& other, bool& carry) const
    {
        bool low_carry = (!m_low && carry) || (m_low - carry) < other.low();
        bool high_carry = (!m_high && low_carry) || (m_high - low_carry) < other.high();

        T lower = m_low - other.low() - carry;
        T higher = m_high - other.high() - low_carry;
        carry = high_carry;

        return { lower, higher };
    }

    constexpr R operator+(const bool& other) const
    {
        bool carry = false; 
        return addc((u8)other, carry);
    }
    template<Unsigned U>
    constexpr R operator+(const U& other) const
    {
        bool carry = false; 
        return addc(other, carry);
    }

    constexpr R operator-(const bool& other) const
    {
        bool carry = false; 
        return subc((u8)other, carry);
    }

    template<Unsigned U>
    constexpr R operator-(const U& other) const
    {
        bool carry = false;
        return subc(other, carry);
    }

    template<Unsigned U>
    constexpr R& operator+=(const U& other)
    {
        *this = *this + other;
        return *this;
    }
    template<Unsigned U>
    constexpr R& operator-=(const U& other)
    {
        *this = *this - other;
        return *this;
    }

    constexpr R operator++()
    {
        auto old = *this;
        *this += 1;
        return old;
    }
    constexpr R& operator++(int)
    {
        *this += 1;
        return *this;
    }
    constexpr R operator--()
    {
        auto old = *this;
        *this -= 1;
        return old;
    }
    constexpr R& operator--(int)
    {
        *this -= 1;
        return *this;
    }

    template<Unsigned U>
    requires(my_size() >= sizeof(U)) constexpr R div_mod(const U& divisor, U& remainder) const
    {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdiv-by-zero"
        if (!divisor) {
            volatile int x = 1;
            volatile int y = 0;
            [[maybe_unused]] volatile int z = x / y;
        }
#pragma GCC diagnostic pop

        if (*this < divisor) {
            remainder = static_cast<U>(*this);
            return 0u;
        }
        if (*this == divisor) {
            remainder = 0u;
            return 1u;
        }
        if (divisor == 1u) {
            remainder = 0u;
            return *this;
        }

        remainder = 0u;
        R quotient = 0u;

        for (ssize_t i = sizeof(R) * 8 - clz() - 1; i >= 0; --i) {
            remainder <<= 1u;
            remainder |= (*this >> (size_t)i) & 1u;
            if (remainder >= divisor) {
                remainder -= divisor;
                quotient |= R { 1u } << (size_t)i;
            }
        }

        return quotient;
    }

    template<Unsigned U>
    constexpr R operator*(U other) const
    {
        R res = 0u;
        R that = *this;
        for (; other != 0u; other >>= 1u) {
            if (other & 1u)
                res += that;
            that <<= 1u;
        }
        return res;
    }

    template<Unsigned U>
    constexpr R operator/(const U& other) const
    {
        U mod { 0u }; 
        return div_mod(other, mod);
    }
    template<Unsigned U>
    constexpr U operator%(const U& other) const
    {
        R res { 0u };
        div_mod(other, res);
        return res;
    }

    template<Unsigned U>
    constexpr R& operator*=(const U& other)
    {
        *this = *this * other;
        return *this;
    }
    template<Unsigned U>
    constexpr R& operator/=(const U& other)
    {
        *this = *this / other;
        return *this;
    }
    template<Unsigned U>
    constexpr R& operator%=(const U& other)
    {
        *this = *this % other;
        return *this;
    }

    constexpr R sqrt() const
    {

        if (*this == 1u)
            return 1u;

        ssize_t shift = (sizeof(R) * 8 - clz()) & ~1ULL;

        R res = 0u;
        while (shift >= 0) {
            res = res << 1u;
            R large_cand = (res | 1u);
            if (*this >> (size_t)shift >= large_cand * large_cand)
                res = large_cand;
            shift -= 2;
        }
        return res;
    }

    constexpr R pow(u64 exp)
    {
        R x1 = *this;
        R x2 = *this * *this;
        u64 exp_copy = exp;
        for (ssize_t i = sizeof(u64) * 8 - __builtin_clzll(exp) - 2; i >= 0; --i) {
            if (exp_copy & 1u) {
                x2 *= x1;
                x1 *= x1;
            } else {
                x1 *= x2;
                x2 *= x2;
            }
            exp_copy >>= 1u;
        }
        return x1;
    }
    template<Unsigned U>
    requires(sizeof(U) > sizeof(u64)) constexpr R pow(U exp)
    {
        
        R x1 = *this;
        R x2 = *this * *this;
        U exp_copy = exp;
        for (ssize_t i = sizeof(U) * 8 - exp().clz() - 2; i >= 0; --i) {
            if (exp_copy & 1u) {
                x2 *= x1;
                x1 *= x1;
            } else {
                x1 *= x2;
                x2 *= x2;
            }
            exp_copy >>= 1u;
        }
        return x1;
    }


}

}