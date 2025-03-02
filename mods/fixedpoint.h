/**
 * @file fixedpoint.h
 * @author Krisna Pranav
 * @brief fixedpoint
 * @version 6.0
 * @date 2024-09-10
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/concept.h>
#include <mods/format.h>
#include <mods/integralmath.h>
#include <mods/numericlimits.h>
#include <mods/types.h>

#ifndef KERNEL
#    include <mods/math.h>
#endif

namespace Mods
{
    /**
     * @tparam precision 
     * @tparam Underlying 
     */
    template<size_t precision, typename Underlying>
    class FixedPoint 
    {
        using This = FixedPoint<precision, Underlying>;
        constexpr static Underlying radix_mask = (static_cast<Underlying>(1) << precision) - 1;

        /**
         * @tparam P 
         * @tparam U 
         */
        template<size_t P, typename U>
        friend class FixedPoint;

    public:
        constexpr FixedPoint() = default;

        /**
         * @tparam I 
         */
        template<Integral I>
        constexpr FixedPoint(I value)
            : m_value(static_cast<Underlying>(value) << precision)
        {
        }

        /**
         * @tparam F 
         */
        template<FloatingPoint F>
        constexpr FixedPoint(F value)
            : m_value(static_cast<Underlying>(value * (static_cast<Underlying>(1) << precision)))
        {
        }

        /**
         * @tparam P 
         * @tparam U 
         */
        template<size_t P, typename U>
        explicit constexpr FixedPoint(FixedPoint<P, U> const& other)
            : m_value(other.template cast_to<precision, Underlying>().m_value)
        {
        }

    #ifndef KERNEL
        /**
         * @tparam F 
         * @return F 
         */
        template<FloatingPoint F>
        explicit ALWAYS_INLINE operator F() const
        {
            return (F)m_value * pow<F>(0.5, precision);
        }
    #endif

        /**
         * @tparam I 
         * @return I 
         */
        template<Integral I>
        explicit constexpr operator I() const
        {
            I value = m_value >> precision;
            
            if (m_value & (1u << (precision - 1))) {
                if (m_value & (radix_mask >> 2u)) {
                    value += (m_value > 0 ? 1 : -1);
                } else {
                    value += value & 1;
                }
            }
            return value;
        }

        /**
         * @return constexpr Underlying 
         */
        constexpr Underlying raw() const
        {
            return m_value;
        }

        /**
         * @return constexpr Underlying& 
         */
        constexpr Underlying& raw()
        {
            return m_value;
        }

        /**
         * @return constexpr This 
         */
        constexpr This fract() const
        {
            return create_raw(m_value & radix_mask);
        }

        /**
         * @return constexpr This 
         */
        constexpr This round() const
        {
            return This { static_cast<Underlying>(*this) };
        }

        /**
         * @return constexpr This 
         */
        constexpr This floor() const
        {
            return create_raw(m_value & ~radix_mask);
        }

        /**
         * @return constexpr This 
         */
        constexpr This ceil() const
        {
            return create_raw((m_value & ~radix_mask)
                + (m_value & radix_mask ? 1 << precision : 0));
        }

        /**
         * @return constexpr This 
         */
        constexpr This trunk() const
        {
            return create_raw((m_value & ~radix_mask)
                + ((m_value & radix_mask)
                        ? (m_value > 0 ? 0 : (1 << precision))
                        : 0));
        }

        /**
         * @return constexpr Underlying 
         */
        constexpr Underlying lround() const 
        { 
            return static_cast<Underlying>(*this); 
        }

        /**
         * @return constexpr Underlying 
         */
        constexpr Underlying lfloor() const 
        { 
            return m_value >> precision; 
        }

        /**
         * @return constexpr Underlying 
         */
        constexpr Underlying lceil() const
        {
            return (m_value >> precision)
                + (m_value & radix_mask ? 1 : 0);
        }

        /**
         * @return constexpr Underlying 
         */
        constexpr Underlying ltrunk() const
        {
            return (m_value >> precision)
                + ((m_value & radix_mask)
                        ? m_value > 0 ? 0 : 1
                        : 0);
        }

        /**
         * @return constexpr This 
         */
        constexpr This log2() const
        {
            This b = create_raw(1 << (precision - 1));
            This y = 0;
            This x = *this;

            if (x.raw() <= 0)
                return create_raw(NumericLimits<Underlying>::min());

            if (x != 1) {
                i32 shift_amount = Mods::log2<Underlying>(x.raw()) - precision;
                if (shift_amount > 0)
                    x >>= shift_amount;
                else
                    x <<= -shift_amount;
                y += shift_amount;
            }

            for (size_t i = 0; i < precision; ++i) {
                x *= x;
                if (x >= 2) {
                    x >>= 1;
                    y += b;
                }
                b >>= 1;
            }

            return y;
        }

        constexpr bool signbit() const requires(IsSigned<Underlying>)
        {
            return m_value >> (sizeof(Underlying) * 8 - 1);
        }

        constexpr This operator-() const requires(IsSigned<Underlying>)
        {
            return create_raw(-m_value);
        }

        /**
         * @param other 
         * @return constexpr This 
         */
        constexpr This operator+(This const& other) const
        {
            return create_raw(m_value + other.m_value);
        }

        /**
         * @param other 
         * @return constexpr This 
         */
        constexpr This operator-(This const& other) const
        {
            return create_raw(m_value - other.m_value);
        }

        /**
         * @param other 
         * @return constexpr This 
         */
        constexpr This operator*(This const& other) const
        {
            Underlying value = m_value * other.raw();
            This ret {};
            ret.raw() = value >> precision;
         
            if (value & (1u << (precision - 1))) {
                if (value & (radix_mask >> 2u)) {
                    ret.raw() += (value > 0 ? 1 : -1);
                } else {
                    ret.raw() += m_value & 1;
                }
            }
            return ret;
        }

        /**
         * @param other 
         * @return constexpr This 
         */
        constexpr This operator/(This const& other) const
        {
            return create_raw((m_value / other.m_value) << (precision));
        }

        /**
         * @tparam I 
         * @param other 
         * @return constexpr This 
         */
        template<Integral I>
        constexpr This operator+(I other) const
        {
            return create_raw(m_value + (other << precision));
        }

        /**
         * @tparam I 
         * @param other 
         * @return constexpr This 
         */
        template<Integral I>
        constexpr This operator-(I other) const
        {
            return create_raw(m_value - (other << precision));
        }

        /**
         * @tparam I 
         * @param other 
         * @return constexpr This 
         */
        template<Integral I>
        constexpr This operator*(I other) const
        {
            return create_raw(m_value * other);
        }

        /**
         * @tparam I 
         * @param other 
         * @return constexpr This 
         */
        template<Integral I>
        constexpr This operator/(I other) const
        {
            return create_raw(m_value / other);
        }

        /**
         * @tparam I 
         * @param other 
         * @return constexpr This 
         */
        template<Integral I>
        constexpr This operator>>(I other) const
        {
            return create_raw(m_value >> other);
        }

        /**
         * @tparam I 
         * @param other 
         * @return constexpr This 
         */
        template<Integral I>
        constexpr This operator<<(I other) const
        {
            return create_raw(m_value << other);
        }

        /**
         * @param other 
         * @return This& 
         */
        This& operator+=(This const& other)
        {
            m_value += other.raw();
            return *this;
        }

        /**
         * @param other 
         * @return This& 
         */
        This& operator-=(This const& other)
        {
            m_value -= other.raw();
            return *this;
        }

        /**
         * @param other 
         * @return This& 
         */
        This& operator*=(This const& other)
        {
            Underlying value = m_value * other.raw();
            m_value = value >> precision;
           
            if (value & (1u << (precision - 1))) {
                if (value & (radix_mask >> 2u)) {
                    m_value += (value > 0 ? 1 : -1);
                } else {
                    m_value += m_value & 1;
                }
            }
            return *this;
        }

        /**
         * @param other 
         * @return This& 
         */
        This& operator/=(This const& other)
        {
            m_value /= other.raw();
            m_value <<= precision;
            return *this;
        }

        /**
         * @tparam I 
         * @param other 
         * @return This& 
         */
        template<Integral I>
        This& operator+=(I other)
        {
            m_value += other << precision;
            return *this;
        }

        /**
         * @tparam I 
         * @param other 
         * @return This& 
         */
        template<Integral I>
        This& operator-=(I other)
        {
            m_value -= other << precision;
            return *this;
        }

        /**
         * @tparam I 
         * @param other 
         * @return This& 
         */
        template<Integral I>
        This& operator*=(I other)
        {
            m_value *= other;
            return *this;
        }

        /**
         * @tparam I 
         * @param other 
         * @return This& 
         */
        template<Integral I>
        This& operator/=(I other)
        {
            m_value /= other;
            return *this;
        }

        /**
         * @tparam I 
         * @param other 
         * @return This& 
         */
        template<Integral I>
        This& operator>>=(I other)
        {
            m_value >>= other;
            return *this;
        }

        /**
         * @tparam I 
         * @param other 
         * @return This& 
         */
        template<Integral I>
        This& operator<<=(I other)
        {
            m_value <<= other;
            return *this;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(This const& other) const 
        { 
            return raw() == other.raw(); 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(This const& other) const 
        { 
            return raw() != other.raw(); 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator>(This const& other) const 
        { 
            return raw() > other.raw(); 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator>=(This const& other) const 
        { 
            return raw() >= other.raw(); 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator<(This const& other) const 
        { 
            return raw() < other.raw(); 
        }
        
        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator<=(This const& other) const 
        { 
            return raw() <= other.raw(); 
        }

        /**
         * @tparam I 
         * @param other 
         * @return true 
         * @return false 
         */
        template<Integral I>
        bool operator==(I other) const
        {
            return m_value >> precision == other && !(m_value & radix_mask);
        }

        /**
         * @tparam I 
         * @param other 
         * @return true 
         * @return false 
         */
        template<Integral I>
        bool operator!=(I other) const
        {
            return (m_value >> precision) != other || m_value & radix_mask;
        }

        /**
         * @tparam I 
         * @param other 
         * @return true 
         * @return false 
         */
        template<Integral I>
        bool operator>(I other) const
        {
            return !(*this <= other);
        }

        /**
         * @tparam I 
         * @param other 
         * @return true 
         * @return false 
         */
        template<Integral I>
        bool operator>=(I other) const
        {
            return !(*this < other);
        }

        /**
         * @tparam I 
         * @param other 
         * @return true 
         * @return false 
         */
        template<Integral I>
        bool operator<(I other) const
        {
            return (m_value >> precision) < other || m_value < (other << precision);
        }

        /**
         * @tparam I 
         * @param other 
         * @return true 
         * @return false 
         */
        template<Integral I>
        bool operator<=(I other) const
        {
            return *this < other || *this == other;
        }

        /**
         * @tparam F 
         * @param other 
         * @return true 
         * @return false 
         */
        template<FloatingPoint F>
        bool operator==(F other) const { return *this == (This)other; }

        /**
         * @tparam F 
         * @param other 
         * @return true 
         * @return false 
         */
        template<FloatingPoint F>
        bool operator!=(F other) const 
        { 
            return *this != (This)other; 
        }

        /**
         * @tparam F 
         * @param other 
         * @return true 
         * @return false 
         */
        template<FloatingPoint F>
        bool operator>(F other) const 
        { 
            return *this > (This)other; 
        }

        /**
         * @tparam F 
         * @param other 
         * @return true 
         * @return false 
         */
        template<FloatingPoint F>
        bool operator>=(F other) const 
        { 
            return *this >= (This)other; 
        }

        /**
         * @tparam F 
         * @param other 
         * @return true 
         * @return false 
         */
        template<FloatingPoint F>
        bool operator<(F other) const 
        { 
            return *this < (This)other; 
        }

        /**
         * @tparam F 
         * @param other 
         * @return true 
         * @return false 
         */
        template<FloatingPoint F>
        bool operator<=(F other) const 
        { 
            return *this <= (This)other; 
        }

        /**
         * @tparam P 
         * @tparam U 
         * @return FixedPoint<P, U> 
         */
        template<size_t P, typename U>
        operator FixedPoint<P, U>() const
        {
            return cast_to<P, U>();
        }

    private:
        /**
         * @tparam P 
         * @tparam U 
         * @return constexpr FixedPoint<P, U> 
         */
        template<size_t P, typename U>
        constexpr FixedPoint<P, U> cast_to() const
        {
            U raw_value = static_cast<U>(m_value >> precision) << P;

            if constexpr (precision > P)
                raw_value |= (m_value & radix_mask) >> (precision - P);
            else if constexpr (precision < P)
                raw_value |= static_cast<U>(m_value & radix_mask) << (P - precision);
            else
                raw_value |= m_value & radix_mask;

            return FixedPoint<P, U>::create_raw(raw_value);
        }

        /**
         * @brief Create a raw object
         * 
         * @param value 
         * @return This 
         */
        static This create_raw(Underlying value)
        {
            This t {};
            t.raw() = value;
            return t;
        }

        Underlying m_value;
    }; // class FixedPoint
} // namespace Mods

using Mods::FixedPoint;
