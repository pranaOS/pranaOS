/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Concepts.h>
#include <base/Math.h>

#ifdef __cplusplus
#    if __cplusplus >= 201103L
#        define COMPLEX_NOEXCEPT noexcept
#    endif
namespace Base {

template<Base::Concepts::Arithmetic T>
class [[gnu::packed]] Complex {
public:
    constexpr Complex()
        : m_real(0)
        , m_imag(0)
    {
    }

    constexpr Complex(T real)
        : m_real(real)
        , m_imag((T)0)
    {
    }

    constexpr Complex(T real, T imaginary)
        : m_real(real)
        , m_imag(imaginary)
    {
    }

    constexpr T real() const COMPLEX_NOEXCEPT { return m_real; }
}

}