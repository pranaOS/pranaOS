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

}

}