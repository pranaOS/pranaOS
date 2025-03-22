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

#include <mods/concepts.h>
#include <mods/format.h>
#include <mods/integralmath.h>
#include <mods/numericlimits.h>
#include <mods/types.h>

#ifndef KERNEL
#    include <mods/math.h>
#endif
#ifndef __SIZEOF_INT128__
#    include <mods/ufixedbigint.h>
#    include <mods/ufixedbigintdivision.h>
#endif

#ifdef MODS_OS_SOLARIS
#    undef signbit
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

    #ifndef KERNEL
        /**
         * @brief Construct a new Fixed Point object
         * 
         * @tparam F 
         * @param value 
         */
        template<FloatingPoint F>
        FixedPoint(F value)
            : m_value(round_to<Underlying>(value * (static_cast<Underlying>(1) << precision)))
        {
        }
    #endif

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
            return trunc().raw() >> precision;
        }

        /**
         * @brief Create a raw object
         * 
         * @param value 
         * @return constexpr This 
         */
        static constexpr This create_raw(Underlying value)
        {
            This t {};
            t.raw() = value;
            return t;
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
         * @param minimum 
         * @param maximum 
         * @return constexpr This 
         */
        constexpr This clamp(This minimum, This maximum) const
        {
            if (*this < minimum)
                return minimum;
            if (*this > maximum)
                return maximum;
            return *this;
        }

        /**
         * @return constexpr This 
         */
        constexpr This rint() const
        {
            Underlying value = m_value >> precision;

            if (m_value & (static_cast<Underlying>(1) << (precision - 1))) {
                if (m_value & (radix_mask >> 1)) {
                    value += 1;
                } else {
                    value += value & 1;
                }
            }
            return value;
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
                + (m_value & radix_mask ? static_cast<Underlying>(1) << precision : 0));
        }

        /**
         * @return constexpr This 
         */
        constexpr This trunc() const
        {
            return create_raw((m_value & ~radix_mask)
                + ((m_value & radix_mask)
                        ? (m_value > 0 ? 0 : (static_cast<Underlying>(1) << precision))
                        : 0));
        }

        /**
         * @return constexpr Underlying 
         */
        constexpr Underlying lrint() const 
        { 
            return rint().raw() >> precision; 
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
        constexpr Underlying ltrunc() const
        {
            return (m_value >> precision)
                + ((m_value & radix_mask)
                        ? m_value > 0 ? 0 : 1
                        : 0);
        }

        constexpr This log2() const
        {
            This b = create_raw(static_cast<Underlying>(1) << (precision - 1));
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

        constexpr bool signbit() const
        requires(IsSigned<Underlying>)
        {
            return m_value >> (sizeof(Underlying) * 8 - 1);
        }

        constexpr This operator-() const
        requires(IsSigned<Underlying>)
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
    #ifdef __SIZEOF_INT128__
            using MulRes = Conditional<sizeof(Underlying) < sizeof(i64), i64, __int128>;
            MulRes value = raw();
            value *= other.raw();

            This ret = create_raw(value >> precision);
    #else
            u128 value = { (u64)(i64)raw(), ~0ull * (raw() < 0) };
            value *= (u64)(i64)other.raw();

            This ret = create_raw((value >> precision).low());
    #endif
            if (value & (static_cast<Underlying>(1) << (precision - 1))) {
                if (value & (radix_mask >> 1)) {
                    ret.raw() += 1;
                } else {
                    ret.raw() += ret.raw() & 1;
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
    #ifdef __SIZEOF_INT128__
            using DivRes = Conditional<sizeof(Underlying) < sizeof(i64), i64, __int128>;

            DivRes value = raw();
            value <<= precision;
            value /= other.raw();

            return create_raw(value);
    #else
            if constexpr (sizeof(Underlying) > sizeof(u32)) {
                u128 value = { (u64)(i64)raw(), ~0ull * (raw() < 0) };

                value <<= precision;
                value /= (u64)(i64)other.raw();

                return create_raw(value.low());
            }

            using DivRes = Conditional<sizeof(Underlying) < sizeof(i32), i32, i64>;
            DivRes value = raw();
            value <<= precision;
            value /= other.raw();

            return create_raw(value);
    #endif
        }

        /**
         * @tparam I 
         * @param other 
         * @return constexpr This 
         */
        template<Integral I>
        constexpr This operator+(I other) const
        {
            return create_raw(m_value + (static_cast<Underlying>(other) << precision));
        }

        /**
         * @tparam I 
         * @param other 
         * @return constexpr This 
         */
        template<Integral I>
        constexpr This operator-(I other) const
        {
            return create_raw(m_value - (static_cast<Underlying>(other) << precision));
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
            *this = *this * other;
            return *this;
        }

        /**
         * @param other 
         * @return This& 
         */
        This& operator/=(This const& other)
        {
            *this = *this / other;
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
            m_value += static_cast<Underlying>(other) << precision;
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
            m_value -= static_cast<Underlying>(other) << precision;
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
            return (m_value >> precision) < other || m_value < (static_cast<Underlying>(other) << precision);
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
        bool operator==(F other) const 
        { 
            return *this == (This)other; 
        }

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

        Underlying m_value;
    }; // class FixedPoint

    /**
     * @tparam precision 
     * @tparam Underlying 
     */
    template<size_t precision, typename Underlying>
    struct Formatter<FixedPoint<precision, Underlying>> : StandardFormatter {
        Formatter() = default;

        /**
         * @brief Construct a new Formatter object
         * 
         * @param formatter 
         */
        explicit Formatter(StandardFormatter formatter)
            : StandardFormatter(formatter)
        {
        }

        /**
         * @param builder 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, FixedPoint<precision, Underlying> value)
        {
            u8 base;
            bool upper_case = false;
            if (m_mode == Mode::Default || m_mode == Mode::FixedPoint) {
                base = 10;
            } else if (m_mode == Mode::Hexfloat) {
                base = 16;
            } else if (m_mode == Mode::HexfloatUppercase) {
                base = 16;
                upper_case = true;
            } else if (m_mode == Mode::Binary) {
                base = 2;
            } else if (m_mode == Mode::BinaryUppercase) {
                base = 2;
                upper_case = true;
            } else if (m_mode == Mode::Octal) {
                base = 8;
            } else {
                VERIFY_NOT_REACHED();
            }

            m_width = m_width.value_or(0);
            m_precision = m_precision.value_or(6);

            bool is_negative = false;
            if constexpr (IsSigned<Underlying>)
                is_negative = value < 0;

            i64 integer = value.ltrunc();
            constexpr u64 one = static_cast<Underlying>(1) << precision;
            u64 fraction_raw = value.raw() & (one - 1);
            return builder.put_fixed_point(is_negative, integer, fraction_raw, one, precision, base, upper_case, m_zero_pad, m_use_separator, m_align, m_width.value(), m_precision.value(), m_fill, m_sign_mode);
        }
    };

} // namespace Mods

#if USING_MODS_GLOBALLY
using Mods::FixedPoint;
#endif
