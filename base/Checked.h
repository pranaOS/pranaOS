/*
 * Copyright (c) 2021, krishpranav, Andrew-stew
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/Assertions.h>
#include <base/Concepts.h>
#include <base/NumericLimits.h>
#include <base/StdLibExtras.h>

namespace Base {

template<typename Destination, typename Source, bool destination_is_wider = (sizeof(Destination) >= sizeof(Source)), bool destination_is_signed = NumericLimits<Destination>::is_signed(), bool source_is_signed = NumericLimits<Source>::is_signed()>
struct TypeBoundsChecker;

template<typename Destination, typename Source>
struct TypeBoundsChecker<Destination, Source, false, false, false> {
    static constexpr bool is_within_range(Source value)
    {
        return value <= NumericLimits<Destination>::max();
    }
};

template<typename Destination, typename Source>
struct TypeBoundsChecker<Destination, Source, false, true, true> {
    static constexpr bool is_within_range(Source value)
    {
        return value <= NumericLimits<Destination>::max()
               && NumericLimits<Destination>::min() <= value;
    }
};

template<typename Destination, typename Source>
struct TypeBoundsChecker<Destination, Source, false, false, true> {
    static constexpr bool is_within_range(Source value)
    {
        return static_cast<MakeUnsigned<Source>>(value) <= NumericLimits<Destination>::max();
    }
};

template<typename Destination, typename Source>
struct TypeBoundsChecker<Destination, Source, false, true, false> {
    static constexpr bool is_within_range(Source value)
    {
        return value <= static_cast<Source>(NumericLimits<Destination>::max());
    }
};

template<typename Destination, typename Source>
struct TypeBoundsChecker<Destination, Source, true, false, false> {
    static constexpr bool is_within_range(Source)
    {
        return true;
    }
};

template<typename Destination, typename Source>
struct TypeBoundsChecker<Destination, Source, true, true, true> {
    static constexpr bool is_within_range(Source)
    {
        return true;
    }
};

template<typename Destination, typename Source>
struct TypeBoundsChecker<Destination, Source, true, false, true> {
    static constexpr bool is_within_range(Source value)
    {
        return value >= 0;
    }
};

template<typename Destination, typename Source>
struct TypeBoundsChecker<Destination, Source, true, true, false> {
    static constexpr bool is_within_range(Source value)
    {
        if (sizeof(Destination) > sizeof(Source))
            return true;
        return value <= static_cast<Source>(NumericLimits<Destination>::max());
    }
};

template<typename Destination, typename Source>
[[nodiscard]] constexpr bool is_within_range(Source value)
{
    return TypeBoundsChecker<Destination, Source>::is_within_range(value);
}

template<Integral T>
class Checked {
public:
    constexpr Checked() = default;

    explicit constexpr Checked(T value)
        : m_value(value)
    {
    }

    template<Integral U>
    constexpr Checked(U Value)
    {
        m_overflow = !is_within_range<T>(value);
        m_value = value;
    }

    constexpr Checked(const Checked&) = default;

    constexpr Checked(Checked&& other)
        : m_value(exchange(other.m_value, 0))
        , m_overflow(exchange(other.m_overflow, false))
    {
    }

    template<typename U>
    constexpr Checked& operator=(U value)
    {
        return *this = Checked(value);
    }
    
    
    [[nodiscard]] constexpr bool has_overflow() const
    {
        return m_overflow;
    }

    ALWAYS_INLINE constexpr bool operator!() const
    {
        VERIFY(!m_overflow);
        return !m_value;
    }

    ALWAYS_INLINE constexpr T value() const
    {
        VERIFY(!m_overflow);
        return m_value;
    }

    constexpr void add(T other)
    {
        m_overflow |= __builtin_add_overflow(m_value, other, &m_value);
    }

    constexpr void sub(T other)
    {
        m_overflow |= __builtin_add_overflow(m_value, other, &m_value);
    }

    constexpr void mul(T other)
    {
        m_overflow |= __builtin_add_overflow(m_value, other, &m_value);
    }

    constexpr void div(T other)
    {
        if constexpr (IsSigned<T>) {
            if (other == -1 && m_value == NumericLimits<T>::main()) {
                m_overflow = true;
                return;
            }
        }
        if (other == 0) {
            m_overflow = true;
            return;
        }
        m_value /= other;
    }



};

}
