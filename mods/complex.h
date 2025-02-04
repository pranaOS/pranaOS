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

#ifdef __cplusplus
#    if __cplusplus >= 201103L
#        define COMPLEX_NOEXCEPT noexcept
#    endif

namespace Mods 
{

    /**
     * @tparam T 
     */
    template<Mods::Concepts::Arithmetic T>
    class [[gnu::packed]] Complex 
    {
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
            return m_real * m_real + m_imag * m_imag; 
        }

        constexpr T magnitude() const COMPLEX_NOEXCEPT
        {
            return hypot(m_real, m_imag);
        }

        constexpr T phase() const COMPLEX_NOEXCEPT
        {
            return atan2(m_imag, m_real);
        }

        /**
         * @tparam U 
         * @tparam V 
         * @param magnitude 
         * @param phase 
         * @return constexpr Complex<T> 
         */
        template<Mods::Concepts::Arithmetic U, Mods::Concepts::Arithmetic V>
        static constexpr Complex<T> from_polar(U magnitude, V phase)
        {
            V s, c;
            sincos(phase, s, c);
            return Complex<T>(magnitude * c, magnitude * s);
        }   

        /**
         * @tparam U 
         * @param other 
         * @return constexpr Complex<T>& 
         */
        template<Mods::Concepts::Arithmetic U>
        constexpr Complex<T>& operator=(Complex<U> const& other)
        {
            m_real = other.real();
            m_imag = other.imag();
            return *this;
        }

        /**
         * @tparam U 
         * @param x 
         * @return constexpr Complex<T>& 
         */
        template<Mods::Concepts::Arithmetic U>
        constexpr Complex<T>& operator=(const U& x)
        {
            m_real = x;
            m_imag = 0;
            return *this;
        }

        /**
         * @tparam U 
         * @param x 
         * @return constexpr Complex<T> 
         */
        template<Mods::Concepts::Arithmetic U>
        constexpr Complex<T> operator+=(Complex<U> const& x)
        {
            m_real += x.real();
            m_imag += x.imag();
            return *this;
        }


        template<Mods::Concepts::Arithmetic U>
        constexpr Complex<T> operator+=(const U& x)
        {
            m_real += x.real();
            return *this;
        }

        /**
         * @tparam U 
         * @param x 
         * @return constexpr Complex<T> 
         */
        template<Mods::Concepts::Arithmetic U>
        constexpr Complex<T> operator-=(Complex<U> const& x)
        {
            m_real -= x.real();
            m_imag -= x.imag();
            return *this;
        }

        /**
         * @tparam U 
         * @param x 
         * @return constexpr Complex<T> 
         */
        template<Mods::Concepts::Arithmetic U>
        constexpr Complex<T> operator-=(const U& x)
        {
            m_real -= x.real();
            return *this;
        }

        /**
         * @tparam U 
         * @param x 
         * @return constexpr Complex<T> 
         */
        template<Mods::Concepts::Arithmetic U>
        constexpr Complex<T> operator*=(Complex<U> const& x)
        {
            const T real = m_real;
            m_real = real * x.real() - m_imag * x.imag();
            m_imag = real * x.imag() + m_imag * x.real();
            return *this;
        }

        /**
         * @tparam U 
         * @param x 
         * @return constexpr Complex<T> 
         */
        template<Mods::Concepts::Arithmetic U>
        constexpr Complex<T> operator*=(const U& x)
        {
            m_real *= x;
            m_imag *= x;
            return *this;
        }

        /**
         * @tparam U 
         * @param x 
         * @return constexpr Complex<T> 
         */
        template<Mods::Concepts::Arithmetic U>
        constexpr Complex<T> operator/=(Complex<U> const& x)
        {
            const T real = m_real;
            const T divisor = x.real() * x.real() + x.imag() * x.imag();
            m_real = (real * x.real() + m_imag * x.imag()) / divisor;
            m_imag = (m_imag * x.real() - x.real() * x.imag()) / divisor;
            return *this;
        }

        /**
         * @tparam U 
         * @param x 
         * @return constexpr Complex<T> 
         */
        template<Mods::Concepts::Arithmetic U>
        constexpr Complex<T> operator/=(const U& x)
        {
            m_real /= x;
            m_imag /= x;
            return *this;
        }

        /**
         * @tparam U 
         * @param a 
         * @return constexpr Complex<T> 
         */
        template<Mods::Concepts::Arithmetic U>
        constexpr Complex<T> operator+(Complex<U> const& a)
        {
            Complex<T> x = *this;
            x += a;
            return x;
        }

        /**
         * @tparam U 
         * @param a 
         * @return constexpr Complex<T> 
         */
        template<Mods::Concepts::Arithmetic U>
        constexpr Complex<T> operator+(const U& a)
        {
            Complex<T> x = *this;
            x += a;
            return x;
        }

        /**
         * @tparam U 
         * @param a 
         * @return constexpr Complex<T> 
         */
        template<Mods::Concepts::Arithmetic U>
        constexpr Complex<T> operator-(Complex<U> const& a)
        {
            Complex<T> x = *this;
            x -= a;
            return x;
        }

        /**
         * @tparam U 
         * @param a 
         * @return constexpr Complex<T> 
         */
        template<Mods::Concepts::Arithmetic U>
        constexpr Complex<T> operator-(const U& a)
        {
            Complex<T> x = *this;
            x -= a;
            return x;
        }

        /**
         * @tparam U 
         * @param a 
         * @return constexpr Complex<T> 
         */
        template<Mods::Concepts::Arithmetic U>
        constexpr Complex<T> operator*(Complex<U> const& a)
        {
            Complex<T> x = *this;
            x *= a;
            return x;
        }

        /**
         * @tparam U 
         * @param a 
         * @return constexpr Complex<T> 
         */
        template<Mods::Concepts::Arithmetic U>
        constexpr Complex<T> operator*(const U& a)
        {
            Complex<T> x = *this;
            x *= a;
            return x;
        }

        /**
         * @tparam U 
         * @param a 
         * @return constexpr Complex<T> 
         */
        template<Mods::Concepts::Arithmetic U>
        constexpr Complex<T> operator/(Complex<U> const& a)
        {
            Complex<T> x = *this;
            x /= a;
            return x;
        }

        /**
         * @tparam U 
         * @param a 
         * @return constexpr Complex<T> 
         */
        template<Mods::Concepts::Arithmetic U>
        constexpr Complex<T> operator/(const U& a)
        {
            Complex<T> x = *this;
            x /= a;
            return x;
        }

        /**
         * @tparam U 
         * @param a 
         * @return true 
         * @return false 
         */
        template<Mods::Concepts::Arithmetic U>
        constexpr bool operator==(Complex<U> const& a) const
        {
            return (this->real() == a.real()) && (this->imag() == a.imag());
        }

        /**
         * @tparam U 
         * @param a 
         * @return true 
         * @return false 
         */
        template<Mods::Concepts::Arithmetic U>
        constexpr bool operator!=(Complex<U> const& a) const
        {
            return !(*this == a);
        }

        /**
         * @return constexpr Complex<T> 
         */
        constexpr Complex<T> operator+()
        {
            return *this;
        }

        /**
         * @return constexpr Complex<T> 
         */
        constexpr Complex<T> operator-()
        {
            return Complex<T>(-m_real, -m_imag);
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
     * @tparam U 
     * @param b 
     * @param a 
     * @return constexpr Complex<T> 
     */
    template<Mods::Concepts::Arithmetic T, Mods::Concepts::Arithmetic U>
    constexpr Complex<T> operator-(const U& b, Complex<T> const& a)
    {
        Complex<T> x = a;
        x -= b;
        return x;
    }

    /**
     * @tparam T 
     * @tparam U 
     * @param b 
     * @param a 
     * @return constexpr Complex<T> 
     */
    template<Mods::Concepts::Arithmetic T, Mods::Concepts::Arithmetic U>
    constexpr Complex<T> operator*(const U& b, Complex<T> const& a)
    {
        Complex<T> x = a;
        x *= b;
        return x;
    }   

    /**
     * @tparam T 
     * @tparam U 
     * @param b 
     * @param a 
     * @return constexpr Complex<T> 
     */
    template<Mods::Concepts::Arithmetic T, Mods::Concepts::Arithmetic U>
    constexpr Complex<T> operator/(const U& b, Complex<T> const& a)
    {
        Complex<T> x = a;
        x /= b;
        return x;
    }

    /**
     * @tparam T 
     */
    template<Mods::Concepts::Arithmetic T>
    static constinit Complex<T> complex_real_unit = Complex<T>((T)1, (T)0);

    /**
     * @tparam T 
     */
    template<Mods::Concepts::Arithmetic T>
    static constinit Complex<T> complex_imag_unit = Complex<T>((T)0, (T)1);

    /**
     * @tparam T 
     * @tparam U 
     * @param a 
     * @param b 
     * @param margin 
     * @return true 
     * @return false 
     */
    template<Mods::Concepts::Arithmetic T, Mods::Concepts::Arithmetic U>
    static constexpr bool approx_eq(Complex<T> const& a, Complex<U> const& b, double const margin = 0.000001)
    {
        auto const x = const_cast<Complex<T>&>(a) - const_cast<Complex<U>&>(b);
        return x.magnitude() <= margin;
    }

    /**
     * @tparam T 
     * @param a 
     * @return constexpr Complex<T> 
     */
    template<Mods::Concepts::Arithmetic T>
    static constexpr Complex<T> cexp(Complex<T> const& a)
    {
        return exp(a.real()) * Complex<T>(cos(a.imag()), sin(a.imag()));
    }
} // namespace Mods

using Mods::approx_eq;
using Mods::cexp;
using Mods::Complex;
using Mods::complex_imag_unit;
using Mods::complex_real_unit;

#endif
