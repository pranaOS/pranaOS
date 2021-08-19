/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <EP/Format.h>
#include <EP/Traits.h>
#include <EP/Types.h>

namespace EP {

template<typename T, typename X, bool Incr, bool Cmp, bool Bool, bool Flags, bool Shift, bool Arith>
class DistinctNumeric {
    using Self = DistinctNumeric<T, X, Incr, Cmp, Bool, Flags, Shift, Arith>;

public:
    constexpr DistinctNumeric()
    {
    }

    constexpr DistinctNumeric(T value)
        : m_value { value }
    {
    }

    constexpr const T& value() const { return m_value; }

    // Always implemented: identity.
    constexpr bool operator==(const Self& other) const
    {
        return this->m_value == other.m_value;
    }
    constexpr bool operator!=(const Self& other) const
    {
        return this->m_value != other.m_value;
    }

    // Only implemented when `Incr` is true:
    constexpr Self& operator++()
    {
        static_assert(Incr, "'++a' is only available for DistinctNumeric types with 'Incr'.");
        this->m_value += 1;
        return *this;
    }
    constexpr Self operator++(int)
    {
        static_assert(Incr, "'a++' is only available for DistinctNumeric types with 'Incr'.");
        Self ret = this->m_value;
        this->m_value += 1;
        return ret;
    }
    constexpr Self& operator--()
    {
        static_assert(Incr, "'--a' is only available for DistinctNumeric types with 'Incr'.");
        this->m_value -= 1;
        return *this;
    }
    constexpr Self operator--(int)
    {
        static_assert(Incr, "'a--' is only available for DistinctNumeric types with 'Incr'.");
        Self ret = this->m_value;
        this->m_value -= 1;
        return ret;
    }

    // Only implemented when `Cmp` is true:
    constexpr bool operator>(const Self& other) const
    {
        static_assert(Cmp, "'a>b' is only available for DistinctNumeric types with 'Cmp'.");
        return this->m_value > other.m_value;
    }
    constexpr bool operator<(const Self& other) const
    {
        static_assert(Cmp, "'a<b' is only available for DistinctNumeric types with 'Cmp'.");
        return this->m_value < other.m_value;
    }
    constexpr bool operator>=(const Self& other) const
    {
        static_assert(Cmp, "'a>=b' is only available for DistinctNumeric types with 'Cmp'.");
        return this->m_value >= other.m_value;
    }
    constexpr bool operator<=(const Self& other) const
    {
        static_assert(Cmp, "'a<=b' is only available for DistinctNumeric types with 'Cmp'.");
        return this->m_value <= other.m_value;
    }
    // 'operator<=>' cannot be implemented. See class comment.

    // Only implemented when `bool` is true:
    constexpr bool operator!() const
    {
        static_assert(Bool, "'!a' is only available for DistinctNumeric types with 'Bool'.");
        return !this->m_value;
    }
    // Intentionally don't define `operator bool() const` here. C++ is a bit
    // overzealos, and whenever there would be a type error, C++ instead tries
    // to convert to a common int-ish type first. `bool` is int-ish, so
    // `operator bool() const` would defy the entire point of this class.

    // Only implemented when `Flags` is true:
    constexpr Self operator~() const
    {
        static_assert(Flags, "'~a' is only available for DistinctNumeric types with 'Flags'.");
        return ~this->m_value;
    }
    constexpr Self operator&(const Self& other) const
    {
        static_assert(Flags, "'a&b' is only available for DistinctNumeric types with 'Flags'.");
        return this->m_value & other.m_value;
    }
    constexpr Self operator|(const Self& other) const
    {
        static_assert(Flags, "'a|b' is only available for DistinctNumeric types with 'Flags'.");
        return this->m_value | other.m_value;
    }
    constexpr Self operator^(const Self& other) const
    {
        static_assert(Flags, "'a^b' is only available for DistinctNumeric types with 'Flags'.");
        return this->m_value ^ other.m_value;
    }
    constexpr Self& operator&=(const Self& other)
    {
        static_assert(Flags, "'a&=b' is only available for DistinctNumeric types with 'Flags'.");
        this->m_value &= other.m_value;
        return *this;
    }
    constexpr Self& operator|=(const Self& other)
    {
        static_assert(Flags, "'a|=b' is only available for DistinctNumeric types with 'Flags'.");
        this->m_value |= other.m_value;
        return *this;
    }
    constexpr Self& operator^=(const Self& other)
    {
        static_assert(Flags, "'a^=b' is only available for DistinctNumeric types with 'Flags'.");
        this->m_value ^= other.m_value;
        return *this;
    }

    // Only implemented when `Shift` is true:
    // TODO: Should this take `int` instead?
    constexpr Self operator<<(const Self& other) const
    {
        static_assert(Shift, "'a<<b' is only available for DistinctNumeric types with 'Shift'.");
        return this->m_value << other.m_value;
    }
    constexpr Self operator>>(const Self& other) const
    {
        static_assert(Shift, "'a>>b' is only available for DistinctNumeric types with 'Shift'.");
        return this->m_value >> other.m_value;
    }
    constexpr Self& operator<<=(const Self& other)
    {
        static_assert(Shift, "'a<<=b' is only available for DistinctNumeric types with 'Shift'.");
        this->m_value <<= other.m_value;
        return *this;
    }
    constexpr Self& operator>>=(const Self& other)
    {
        static_assert(Shift, "'a>>=b' is only available for DistinctNumeric types with 'Shift'.");
        this->m_value >>= other.m_value;
        return *this;
    }

    // Only implemented when `Arith` is true:
    constexpr Self operator+(const Self& other) const
    {
        static_assert(Arith, "'a+b' is only available for DistinctNumeric types with 'Arith'.");
        return this->m_value + other.m_value;
    }
    constexpr Self operator-(const Self& other) const
    {
        static_assert(Arith, "'a-b' is only available for DistinctNumeric types with 'Arith'.");
        return this->m_value - other.m_value;
    }
    constexpr Self operator+() const
    {
        static_assert(Arith, "'+a' is only available for DistinctNumeric types with 'Arith'.");
        return +this->m_value;
    }
    constexpr Self operator-() const
    {
        static_assert(Arith, "'-a' is only available for DistinctNumeric types with 'Arith'.");
        return -this->m_value;
    }
    constexpr Self operator*(const Self& other) const
    {
        static_assert(Arith, "'a*b' is only available for DistinctNumeric types with 'Arith'.");
        return this->m_value * other.m_value;
    }
    constexpr Self operator/(const Self& other) const
    {
        static_assert(Arith, "'a/b' is only available for DistinctNumeric types with 'Arith'.");
        return this->m_value / other.m_value;
    }
    constexpr Self operator%(const Self& other) const
    {
        static_assert(Arith, "'a%b' is only available for DistinctNumeric types with 'Arith'.");
        return this->m_value % other.m_value;
    }
    constexpr Self& operator+=(const Self& other)
    {
        static_assert(Arith, "'a+=b' is only available for DistinctNumeric types with 'Arith'.");
        this->m_value += other.m_value;
        return *this;
    }
    constexpr Self& operator-=(const Self& other)
    {
        static_assert(Arith, "'a+=b' is only available for DistinctNumeric types with 'Arith'.");
        this->m_value += other.m_value;
        return *this;
    }
    constexpr Self& operator*=(const Self& other)
    {
        static_assert(Arith, "'a*=b' is only available for DistinctNumeric types with 'Arith'.");
        this->m_value *= other.m_value;
        return *this;
    }
    constexpr Self& operator/=(const Self& other)
    {
        static_assert(Arith, "'a/=b' is only available for DistinctNumeric types with 'Arith'.");
        this->m_value /= other.m_value;
        return *this;
    }
    constexpr Self& operator%=(const Self& other)
    {
        static_assert(Arith, "'a%=b' is only available for DistinctNumeric types with 'Arith'.");
        this->m_value %= other.m_value;
        return *this;
    }

private:
    T m_value {};
};

template<typename T, typename X, bool Incr, bool Cmp, bool Bool, bool Flags, bool Shift, bool Arith>
struct Formatter<DistinctNumeric<T, X, Incr, Cmp, Bool, Flags, Shift, Arith>> : Formatter<FormatString> {
    void format(FormatBuilder& builder, DistinctNumeric<T, X, Incr, Cmp, Bool, Flags, Shift, Arith> value)
    {
        return Formatter<FormatString>::format(builder, "{}", value.value());
    }
};

// TODO: When 'consteval' sufficiently-well supported by host compilers, try to
// provide a more usable interface like this one:
// https://gist.github.com/alimpfard/a3b750e8c3a2f44fb3a2d32038968ddf

}

#define TYPEDEF_DISTINCT_NUMERIC_GENERAL(T, Incr, Cmp, Bool, Flags, Shift, Arith, NAME) \
    using NAME = DistinctNumeric<T, struct __##NAME##_tag, Incr, Cmp, Bool, Flags, Shift, Arith>;
#define TYPEDEF_DISTINCT_ORDERED_ID(T, NAME) TYPEDEF_DISTINCT_NUMERIC_GENERAL(T, false, true, true, false, false, false, NAME)
// TODO: Further type aliases?

template<typename T, typename X, auto... Args>
struct Traits<EP::DistinctNumeric<T, X, Args...>> : public GenericTraits<EP::DistinctNumeric<T, X, Args...>> {
    static constexpr bool is_trivial() { return true; }
    static constexpr auto hash(const DistinctNumeric<T, X, Args...>& d) { return Traits<T>::hash(d.value()); }
};

using EP::DistinctNumeric;
