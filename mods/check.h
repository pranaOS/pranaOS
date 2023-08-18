/**
 * @file check.h
 * @author Krisna Pranav
 * @brief Checked
 * @version 6.0
 * @date 2023-06-29
 * 
 * @copyright 2011-2019 Apple Inc. All rights reserved, 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "assertions.h"
#include "numericlimits.h"
#include "stdlibextra.h"

namespace Mods {

    template<typename Destination, typename Source, bool destination_is_wider = (sizeof(Destination) >= sizeof(Source)), bool destination_is_signed = NumericLimits<Destination>::is_signed(), bool source_is_signed = NumericLimits<Source>::is_signed()>
    struct TypeBoundsChecker;
    
    /**
     * @brief within_range
     * 
     * @tparam Destination 
     * @tparam Source 
     */
    template<typename Destination, typename Source>
    struct TypeBoundsChecker<Destination, Source, false, false, false> {
        static constexpr bool is_within_range(Source value) {
            return value <= NumericLimits<Destination>::max();
        }
    };
    
    /**
     * @tparam Destination 
     * @tparam Source 
     */
    template<typename Destination, typename Source>
    struct TypeBoundsChecker<Destination, Source, false, true, true> {
        static constexpr bool is_within_range(Source value) {
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
        static constexpr bool is_within_range(Source value) {
            return static_cast<MakeUnsigned<Source>>(value) <= NumericLimits<Destination>::max();
        }
    };
    
    /**
     * @tparam Destination 
     * @tparam Source 
     */
    template<typename Destination, typename Source>
    struct TypeBoundsChecker<Destination, Source, false, true, false> {
        static constexpr bool is_within_range(Source value) {
            return value <= static_cast<Source>(NumericLimits<Destination>::max());
        }
    };
    
    /**
     * @tparam Destination 
     * @tparam Source 
     */
    template<typename Destination, typename Source>
    struct TypeBoundsChecker<Destination, Source, true, false, false> {
        static constexpr bool is_within_range(Source) {
            return true;
        }
    };

    /**
     * @tparam Destination 
     * @tparam Source 
     */
    template<typename Destination, typename Source>
    struct TypeBoundsChecker<Destination, Source, true, true, true> {
        static constexpr bool is_within_range(Source) {
            return true;
        }
    };

    /**
     * @tparam Destination 
     * @tparam Source 
     */
    template<typename Destination, typename Source>
    struct TypeBoundsChecker<Destination, Source, true, false, true> {
        static constexpr bool is_within_range(Source value) {
            return value >= 0;
        }
    };

    /**
     * @tparam Destination 
     * @tparam Source 
     */
    template<typename Destination, typename Source>
    struct TypeBoundsChecker<Destination, Source, true, true, false> {
        static constexpr bool is_within_range(Source value) {
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
    constexpr bool is_within_range(Source value) {
        return TypeBoundsChecker<Destination, Source>::is_within_range(value);
    }

    template<typename T>
    class Checked {
    public:
        constexpr Checked() = default;

        constexpr Checked(T value)
            : m_value(value)
        {
        }

        /**
         * @tparam U 
         */
        template<typename U>
        constexpr Checked(U value) {
            m_overflow = !is_within_range<T>(value);
            m_value = value;
        }

        constexpr Checked(const Checked&) = default;

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
        constexpr Checked& operator=(U value) {
            return *this = Checked(value);
        }

        constexpr Checked& operator=(const Checked& other) = default;

        /**
         * @param other 
         * @return constexpr Checked& 
         */
        constexpr Checked& operator=(Checked&& other) {
            m_value = exchange(other.m_value, 0);
            m_overflow = exchange(other.m_overflow, false);
            return *this;
        }

        /**
         * @brief has_overflow
         * 
         * @return true 
         * @return false 
         */
        constexpr bool has_overflow() const {
            return m_overflow;
        }

        /**
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr bool operator!() const {
            ASSERT(!m_overflow);
            return !m_value;
        }

        /**
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr T value() const {
            ASSERT(!m_overflow);
            return m_value;
        }

        /**
         * @param other 
         */
        constexpr void add(T other) {
            m_overflow |= __builtin_add_overflow(m_value, other, &m_value);
        }

        /**
         * @param other 
         */
        constexpr void sub(T other) {
            m_overflow |= __builtin_sub_overflow(m_value, other, &m_value);
        }

        /**
         * @param other 
         */
        constexpr void mul(T other) {
            m_overflow |= __builtin_mul_overflow(m_value, other, &m_value);
        }

        /**
         * @param other 
         */
        constexpr void div(T other) {
            m_value /= other;
        }

        /**
         * @param other 
         * @return constexpr Checked& 
         */
        constexpr Checked& operator+=(T other) {
            add(other);
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Checked& 
         */
        constexpr Checked& operator-=(T other) {
            sub(other);
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Checked& 
         */
        constexpr Checked& operator*=(T other) {
            mul(other);
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Checked& 
         */
        constexpr Checked& operator/=(T other) {
            div(other);
            return *this;
        }

        /**
         * @return constexpr Checked& 
         */
        constexpr Checked& operator++() {
            add(1);
            return *this;
        }

        /**
         * @return constexpr Checked 
         */
        constexpr Checked operator++(int) {
            Checked old { *this };
            add(1);
            return old;
        }

        template<typename U, typename V>
        static constexpr bool addition_would_overflow(U u, V v) {
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
        static constexpr bool multiplication_would_overflow(U u, V v) {
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
        static constexpr bool multiplication_would_overflow(U u, V v, X x) {
            Checked checked;
            checked = u;
            checked *= v;
            checked *= x;
            return checked.has_overflow();
        }

    private:
        T m_value {};
        bool m_overflow { false };
    };

    /**
     * @tparam T 
     * @param a 
     * @param b 
     * @return constexpr Checked<T> 
     */
    template<typename T>
    constexpr Checked<T> operator+(const Checked<T>& a, const Checked<T>& b) {
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
    constexpr Checked<T> operator-(const Checked<T>& a, const Checked<T>& b) {
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
    constexpr Checked<T> operator*(const Checked<T>& a, const Checked<T>& b) {
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
    constexpr Checked<T> operator/(const Checked<T>& a, const Checked<T>& b) {
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
    constexpr bool operator<(const Checked<T>& a, T b) {
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
    constexpr bool operator>(const Checked<T>& a, T b) {
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
    constexpr bool operator>=(const Checked<T>& a, T b) {
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
    constexpr bool operator<=(const Checked<T>& a, T b) {
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
    constexpr bool operator==(const Checked<T>& a, T b) {
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
    constexpr bool operator!=(const Checked<T>& a, T b) {
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
    constexpr bool operator<(T a, const Checked<T>& b) {
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
    constexpr bool operator>(T a, const Checked<T>& b) {
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
    constexpr bool operator>=(T a, const Checked<T>& b) {
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
    constexpr bool operator<=(T a, const Checked<T>& b) {
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
    constexpr bool operator==(T a, const Checked<T>& b) {
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
    constexpr bool operator!=(T a, const Checked<T>& b) {
        return a != b.value();
    }

    /**
     * @tparam T 
     * @param value 
     * @return constexpr Checked<T> 
     */
    template<typename T>
    constexpr Checked<T> make_checked(T value) {
        return Checked<T>(value);
    }

}

using Mods::Checked;
using Mods::make_checked;

