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

}

}