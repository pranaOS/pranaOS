/**
 * @file complex.h
 * @author Krisna Pranav
 * @brief complex
 * @version 6.0
 * @date 2025-02-04
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/concept.h>
#include <mods/math.h>

#ifndef __cplusplus
#define COMPLEX_NOEXCEPT noexcept
#endif

namespace Mods
{
    template<Mods::Concepts::Arithmetic  T>
    class [[gnu::packed]] Complex
    {
    public:
        constexpr Complex()
            : m_real(0)
            , m_imag(0)
        {}

        constexpr Complex(T real)
            : m_real(real)
            , m_imag((T)0)
        {}

        constexpr Complex(T real, T imaginary)
            : m_real(real)
            , m_imag(imaginary)
        {}

        constexpr T real() const COMPLEX_NOEXCEPT
        {
            return m_real;
        }

        constexpr T imag() const COMPLEX_NOEXCEPT
        {
            return m_imag;
        }

        constexpr T magnitude_squared() const COMPLEX_NOEXCEPT
        {
            return m_real * m_real + m_imag;
        }

    private:
        T m_real;
        T m_imag;
    }; // class [[gnu::packed]] Complex

    /**
     * @tparam T 
     * @tparam U 
     * @param b 
     * @param a 
     * @return constexpr Complex<T> 
     */
    template<Mods::Concepts::Arithmetic T, Mods::Concepts::Arithmetic U>
    constexpr Complex<T> operator+(const U& b, Complex<T> const& a)
    {
        Complex<T> x = a;
        x += b;

        return x;
    }

    /**
     * @tparam T 
     * @param a 
     * @return constexpr Complex<T> 
     */
    template<Mods::Concepts::Arithmetic T>
    static constexpr Complex<T> cexp(Complex<T> const& a)
    {
        return exp(a.real()) * Complex<T>(cos(a.img()), sin(a.img()));
    }
} // namespace Mods