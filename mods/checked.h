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
#include <mods/concept.h>
#include <mods/numericlimits.h>
#include <mods/stdlibextra.h>

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
    struct TypeBoundsChecker<Destination, Source, false, false, false> 
    {
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
    struct TypeBoundsChecker<Destination, Source, false, true, true> 
    {
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
    struct TypeBoundsChecker<Destination, Source, false, false, true> 
    {
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
    struct TypeBoundsChecker<Destination, Source, false, true, false> 
    {
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
    struct TypeBoundsChecker<Destination, Source, true, false, false> 
    {
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
    struct TypeBoundsChecker<Destination, Source, true, true, true> 
    {
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
    struct TypeBoundsChecker<Destination, Source, true, false, true> 
    {
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
    struct TypeBoundsChecker<Destination, Source, true, true, false> 
    {
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
         * @return value
         * 
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
         * @return constexpr Checked& 
         */
        constexpr Checked& operator++()
        {
            add(1);
            return *this;
        }

        /**
         * @return constexpr Checked 
         */
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

        /**
         * @return constexpr Checked 
         */
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
    #ifdef __clang__
            Checked checked;
            checked = u;
            checked += v;
            return checked.has_overflow();
    #else
            return __builtin_add_overflow_p(u, v, (T)0);
    #endif
        }

        template<typename U, typename V>
        [[nodiscard]] static constexpr bool multiplication_would_overflow(U u, V v)
        {
    #ifdef __clang__
            Checked checked;
            checked = u;
            checked *= v;
            return checked.has_overflow();
    #else
            return __builtin_mul_overflow_p(u, v, (T)0);
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

using Mods::Checked;
using Mods::make_checked;
