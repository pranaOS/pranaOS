/**
 * @file check.h
 * @author Krisna Pranav
 * @brief Checked
 * @version 6.0
 * @date 2023-06-29
 * 
 * @copyright 2011-2019 Apple Inc. All rights reserved, 
 * @copyright 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/assertions.h>
#include <mods/concepts.h>
#include <mods/numericlimits.h>
#include <mods/stdlibextras.h>

namespace Mods 
{

    /**
     * @tparam Destination 
     * @tparam Source 
     * @tparam destination_is_wider 
     */
    template<typename Destination, typename Source, bool destination_is_wider = (sizeof(Destination) >= sizeof(Source)), bool destination_is_signed = NumericLimits<Destination>::is_signed(), bool source_is_signed = NumericLimits<Source>::is_signed()>
    struct TypeBoundsChecker;

    /**
     * @tparam Destination 
     * @tparam Source 
     */
    template<typename Destination, typename Source>
    struct TypeBoundsChecker<Destination, Source, false, false, false> {
        /**
         * @param value 
         * @return true 
         * @return false 
         */
        static constexpr bool is_within_range(Source value)
        {
            return value <= NumericLimits<Destination>::max();
        }
    };

    /**
     * @tparam Destination 
     * @tparam Source 
     */
    template<typename Destination, typename Source>
    struct TypeBoundsChecker<Destination, Source, false, true, true> {
        /**
         * @param value 
         * @return true 
         * @return false 
         */
        static constexpr bool is_within_range(Source value)
        {
            return value <= NumericLimits<Destination>::max()
                && NumericLimits<Destination>::min() <= value;
        }
    };

    /**
     * @tparam Destination 
     * @tparam Source 
     */
    template<typename Destination, typename Source>
    struct TypeBoundsChecker<Destination, Source, false, false, true> {
        /**
         * @param value 
         * @return true 
         * @return false 
         */
        static constexpr bool is_within_range(Source value)
        {
            return value >= 0 && value <= NumericLimits<Destination>::max();
        }
    };

    /**
     * @tparam Destination 
     * @tparam Source 
     */
    template<typename Destination, typename Source>
    struct TypeBoundsChecker<Destination, Source, false, true, false> {

        /**
         * @param value 
         * @return true 
         * @return false 
         */
        static constexpr bool is_within_range(Source value)
        {
            return value <= static_cast<Source>(NumericLimits<Destination>::max());
        }
    };

    /**
     * @tparam Destination 
     * @tparam Source 
     */
    template<typename Destination, typename Source>
    struct TypeBoundsChecker<Destination, Source, true, false, false> {
        /**
         * @return true 
         * @return false 
         */
        static constexpr bool is_within_range(Source)
        {
            return true;
        }
    };

    /**
     * @tparam Destination 
     * @tparam Source 
     */
    template<typename Destination, typename Source>
    struct TypeBoundsChecker<Destination, Source, true, true, true> {
        static constexpr bool is_within_range(Source)
        {
            return true;
        }
    };

    /**
     * @tparam Destination 
     * @tparam Source 
     */
    template<typename Destination, typename Source>
    struct TypeBoundsChecker<Destination, Source, true, false, true> {
        /**
         * @param value 
         * @return true 
         * @return false 
         */
        static constexpr bool is_within_range(Source value)
        {
            return value >= 0;
        }
    };

    /**
     * @tparam Destination 
     * @tparam Source 
     */
    template<typename Destination, typename Source>
    struct TypeBoundsChecker<Destination, Source, true, true, false> {
        /**
         * @param value 
         * @return true 
         * @return false 
         */
        static constexpr bool is_within_range(Source value)
        {
            if (sizeof(Destination) > sizeof(Source))
                return true;
            return value <= static_cast<Source>(NumericLimits<Destination>::max());
        }
    };

    /**
     * @tparam Destination 
     * @tparam Source 
     * @param value 
     * @return true 
     * @return false 
     */
    template<typename Destination, typename Source>
    [[nodiscard]] constexpr bool is_within_range(Source value)
    {
        return TypeBoundsChecker<Destination, Source>::is_within_range(value);
    }

    /**
     * @tparam T 
     */
    template<Integral T>
    class Checked 
    {
    public:
        constexpr Checked() = default;

        /**
         * @param value
         */
        explicit constexpr Checked(T value)
            : m_value(value)
        {
        }

        /**
         * @tparam U 
         */
        template<Integral U>
        constexpr Checked(U value)
        {
            m_overflow = !is_within_range<T>(value);
            m_value = value;
        }

        constexpr Checked(Checked const&) = default;

        /**
         * @param other
         */
        constexpr Checked(Checked&& other)
            : m_value(exchange(other.m_value, 0))
            , m_overflow(exchange(other.m_overflow, false))
        {
        }

        /**
         * @tparam U 
         * @param value 
         * @return constexpr Checked& 
         */
        template<typename U>
        constexpr Checked& operator=(U value)
        {
            *this = Checked(value);
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Checked& 
         */
        constexpr Checked& operator=(Checked const& other) = default;

        /**
         * @param other 
         * @return constexpr Checked& 
         */
        constexpr Checked& operator=(Checked&& other)
        {
            m_value = exchange(other.m_value, 0);
            m_overflow = exchange(other.m_overflow, false);
            return *this;
        }

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] constexpr bool has_overflow() const
        {
            return m_overflow;
        }

        /**
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr bool operator!() const
        {
            VERIFY(!m_overflow);
            return !m_value;
        }

        /**
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr T value() const
        {
            VERIFY(!m_overflow);
            return m_value;
        }

        /**
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr T value_unchecked() const
        {
            return m_value;
        }

        /**
         * @param other 
         */
        constexpr void add(T other)
        {
            m_overflow |= __builtin_add_overflow(m_value, other, &m_value);
        }

        /**
         * @param other 
         */
        constexpr void sub(T other)
        {
            m_overflow |= __builtin_sub_overflow(m_value, other, &m_value);
        }

        /**
         * @param other 
         */
        constexpr void mul(T other)
        {
            m_overflow |= __builtin_mul_overflow(m_value, other, &m_value);
        }

        /**
         * @param other 
         */
        constexpr void div(T other)
        {
            if constexpr (IsSigned<T>) {
                if (other == -1 && m_value == NumericLimits<T>::min()) {
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

        /**
         * @param other 
         */
        constexpr void mod(T other)
        {
            auto initial = m_value;
            div(other);
            m_value *= other;
            m_value = initial - m_value;
        }

        /**
         * @param other 
         */
        constexpr void saturating_sub(T other)
        {
            sub(other);

            if (m_overflow && other <= 0)
                m_value = NumericLimits<T>::max();
            else if (m_overflow)
                m_value = NumericLimits<T>::min();
            m_overflow = false;
        }

        /**
         * @param other 
         */
        constexpr void saturating_add(T other)
        {
            add(other);

            if (m_overflow && other >= 0)
                m_value = NumericLimits<T>::max();
            else if (m_overflow)
                m_value = NumericLimits<T>::min();
            m_overflow = false;
        }

        /**
         * @param other 
         */
        constexpr void saturating_mul(T other)
        {
            auto either_is_zero = this->m_value == 0 || other == 0;
            auto result_is_positive = (this->m_value > 0) == (other > 0);

            mul(other);
            if (m_overflow) {
                if (either_is_zero)
                    m_value = 0;
                else if (result_is_positive)
                    m_value = NumericLimits<T>::max();
                else
                    m_value = NumericLimits<T>::min();
            }
            m_overflow = false;
        }

        /**
         * @param other 
         * @return constexpr Checked& 
         */
        constexpr Checked& operator+=(Checked const& other)
        {
            m_overflow |= other.m_overflow;
            add(other.value());
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Checked& 
         */
        constexpr Checked& operator+=(T other)
        {
            add(other);
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Checked& 
         */
        constexpr Checked& operator-=(Checked const& other)
        {
            m_overflow |= other.m_overflow;
            sub(other.value());
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Checked& 
         */
        constexpr Checked& operator-=(T other)
        {
            sub(other);
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Checked& 
         */
        constexpr Checked& operator*=(Checked const& other)
        {
            m_overflow |= other.m_overflow;
            mul(other.value());
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Checked& 
         */
        constexpr Checked& operator*=(T other)
        {
            mul(other);
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Checked& 
         */
        constexpr Checked& operator/=(Checked const& other)
        {
            m_overflow |= other.m_overflow;
            div(other.value());
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Checked& 
         */
        constexpr Checked& operator/=(T other)
        {
            div(other);
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Checked& 
         */
        constexpr Checked& operator%=(Checked const& other)
        {
            m_overflow |= other.m_overflow;
            mod(other.value());
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Checked& 
         */
        constexpr Checked& operator%=(T other)
        {
            mod(other);
            return *this;
        }

        /**
         * @return constexpr Checked& 
         */
        constexpr Checked& operator++()
        {
            add(1);
            return *this;
        }

        constexpr Checked operator++(int)
        {
            Checked old { *this };
            add(1);
            return old;
        }

        /**
         * @return constexpr Checked& 
         */
        constexpr Checked& operator--()
        {
            sub(1);
            return *this;
        }

        constexpr Checked operator--(int)
        {
            Checked old { *this };
            sub(1);
            return old;
        }

        /**
         * @tparam U 
         * @tparam V 
         * @param u 
         * @param v 
         * @return true 
         * @return false 
         */
        template<typename U, typename V>
        [[nodiscard]] static constexpr bool addition_would_overflow(U u, V v)
        {
    #if __has_builtin(__builtin_add_overflow_p)
            return __builtin_add_overflow_p(u, v, (T)0);
    #elif __has_builtin(__builtin_add_overflow)
            T result;
            return __builtin_add_overflow(u, v, &result);
    #else
            Checked checked;
            checked = u;
            checked += v;
            return checked.has_overflow();
    #endif
        }

        /**
         * @tparam U 
         * @tparam V 
         * @param u 
         * @param v 
         * @return true 
         * @return false 
         */
        template<typename U, typename V>
        [[nodiscard]] static constexpr bool subtraction_would_overflow(U u, V v)
        {
    #if __has_builtin(__builtin_sub_overflow_p)
            return __builtin_sub_overflow_p(u, v, (T)0);
    #elif __has_builtin(__builtin_sub_overflow)
            T result;
            return __builtin_sub_overflow(u, v, &result);
    #else
            Checked checked;
            checked = u;
            checked -= v;
            return checked.has_overflow();
    #endif
        }

        /**
         * @tparam U 
         * @tparam V 
         * @param a 
         * @param b 
         * @return constexpr T 
         */
        template<typename U, typename V>
        static constexpr T saturating_add(U a, V b)
        {
            Checked checked { a };
            checked.saturating_add(b);
            return checked.value();
        }

        /**
         * @tparam U 
         * @tparam V 
         * @param a 
         * @param b 
         * @return constexpr T 
         */
        template<typename U, typename V>
        static constexpr T saturating_sub(U a, V b)
        {
            Checked checked { a };
            checked.saturating_sub(b);
            return checked.value();
        }

        /**
         * @tparam U 
         * @tparam V 
         * @param a 
         * @param b 
         * @return constexpr T 
         */
        template<typename U, typename V>
        static constexpr T saturating_mul(U a, V b)
        {
            Checked checked { a };
            checked.saturating_mul(b);
            return checked.value();
        }

        /**
         * @tparam U 
         * @tparam V 
         * @param u 
         * @param v 
         * @return true 
         * @return false 
         */
        template<typename U, typename V>
        [[nodiscard]] static constexpr bool multiplication_would_overflow(U u, V v)
        {
    #if __has_builtin(__builtin_mul_overflow_p)
            return __builtin_mul_overflow_p(u, v, (T)0);
    #elif __has_builtin(__builtin_mul_overflow)
            T result;
            return __builtin_mul_overflow(u, v, &result);
    #else
            Checked checked;
            checked = u;
            checked *= v;
            return checked.has_overflow();
    #endif
        }

        /**
         * @tparam U 
         * @tparam V 
         * @tparam X 
         * @param u 
         * @param v 
         * @param x 
         * @return true 
         * @return false 
         */
        template<typename U, typename V, typename X>
        [[nodiscard]] static constexpr bool multiplication_would_overflow(U u, V v, X x)
        {
            Checked checked;
            checked = u;
            checked *= v;
            checked *= x;
            return checked.has_overflow();
        }

    private:
        T m_value {};
        bool m_overflow { false };
    }; // class Checked

    /**
     * @tparam T 
     * @param a 
     * @param b 
     * @return constexpr Checked<T> 
     */
    template<typename T>
    constexpr Checked<T> operator+(Checked<T> const& a, Checked<T> const& b)
    {
        Checked<T> c { a };
        c.add(b.value());
        return c;
    }

    /**
     * @tparam T 
     * @param a 
     * @param b 
     * @return constexpr Checked<T> 
     */
    template<typename T>
    constexpr Checked<T> operator-(Checked<T> const& a, Checked<T> const& b)
    {
        Checked<T> c { a };
        c.sub(b.value());
        return c;
    }

    /**
     * @tparam T 
     * @param a 
     * @param b 
     * @return constexpr Checked<T> 
     */
    template<typename T>
    constexpr Checked<T> operator*(Checked<T> const& a, Checked<T> const& b)
    {
        Checked<T> c { a };
        c.mul(b.value());
        return c;
    }

    /**
     * @tparam T 
     * @param a 
     * @param b 
     * @return constexpr Checked<T> 
     */
    template<typename T>
    constexpr Checked<T> operator/(Checked<T> const& a, Checked<T> const& b)
    {
        Checked<T> c { a };
        c.div(b.value());
        return c;
    }

    /**
     * @tparam T 
     * @param a 
     * @param b 
     * @return constexpr Checked<T> 
     */
    template<typename T>
    constexpr Checked<T> operator%(Checked<T> const& a, Checked<T> const& b)
    {
        Checked<T> c { a };
        c.mod(b.value());
        return c;
    }

    /**
     * @tparam T 
     * @param a 
     * @param b 
     * @return true 
     * @return false 
     */
    template<typename T>
    constexpr bool operator<(Checked<T> const& a, T b)
    {
        return a.value() < b;
    }

    /**
     * @tparam T 
     * @param a 
     * @param b 
     * @return true 
     * @return false 
     */
    template<typename T>
    constexpr bool operator>(Checked<T> const& a, T b)
    {
        return a.value() > b;
    }

    /**
     * @tparam T 
     * @param a 
     * @param b 
     * @return true 
     * @return false 
     */
    template<typename T>
    constexpr bool operator>=(Checked<T> const& a, T b)
    {
        return a.value() >= b;
    }

    /**
     * @tparam T 
     * @param a 
     * @param b 
     * @return true 
     * @return false 
     */
    template<typename T>
    constexpr bool operator<=(Checked<T> const& a, T b)
    {
        return a.value() <= b;
    }

    /**
     * @tparam T 
     * @param a 
     * @param b 
     * @return true 
     * @return false 
     */
    template<typename T>
    constexpr bool operator==(Checked<T> const& a, T b)
    {
        return a.value() == b;
    }

    /**
     * @tparam T 
     * @param a 
     * @param b 
     * @return true 
     * @return false 
     */
    template<typename T>
    constexpr bool operator!=(Checked<T> const& a, T b)
    {
        return a.value() != b;
    }

    /**
     * @tparam T 
     * @param a 
     * @param b 
     * @return true 
     * @return false 
     */
    template<typename T>
    constexpr bool operator<(T a, Checked<T> const& b)
    {
        return a < b.value();
    }

    /**
     * @tparam T 
     * @param a 
     * @param b 
     * @return true 
     * @return false 
     */
    template<typename T>
    constexpr bool operator>(T a, Checked<T> const& b)
    {
        return a > b.value();
    }

    /**
     * @tparam T 
     * @param a 
     * @param b 
     * @return true 
     * @return false 
     */
    template<typename T>
    constexpr bool operator>=(T a, Checked<T> const& b)
    {
        return a >= b.value();
    }

    /**
     * @tparam T 
     * @param a 
     * @param b 
     * @return true 
     * @return false 
     */
    template<typename T>
    constexpr bool operator<=(T a, Checked<T> const& b)
    {
        return a <= b.value();
    }

    /**
     * @tparam T 
     * @param a 
     * @param b 
     * @return true 
     * @return false 
     */
    template<typename T>
    constexpr bool operator==(T a, Checked<T> const& b)
    {
        return a == b.value();
    }

    /**
     * @tparam T 
     * @param a 
     * @param b 
     * @return true 
     * @return false 
     */
    template<typename T>
    constexpr bool operator!=(T a, Checked<T> const& b)
    {
        return a != b.value();
    }

    /**
     * @tparam T 
     * @param value 
     * @return constexpr Checked<T> 
     */
    template<typename T>
    constexpr Checked<T> make_checked(T value)
    {
        return Checked<T>(value);
    }

} // namespace Mods
  
#if USING_MODS_GLOBALLY
using Mods::Checked;
using Mods::make_checked;
#endif
