/**
 * @file distinctnums.h
 * @author Krisna Pranav
 * @brief distinctnums
 * @version 6.0
 * @date 2023-07-10
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/format.h>
#include <mods/traits.h>
#include <mods/types.h>

namespace Mods
{
    /**
     * @tparam T 
     * @tparam X 
     * @tparam Incr 
     * @tparam Cmp 
     * @tparam Bool 
     * @tparam Flags 
     * @tparam Shift 
     * @tparam Arith 
     */
    template<typename T, typename X, bool Incr, bool Cmp, bool Bool, bool Flags, bool Shift, bool Arith>
    class DistinctNumeric 
    {
        using Self = DistinctNumeric<T, X, Incr, Cmp, Bool, Flags, Shift, Arith>;

    public:
        constexpr DistinctNumeric() = default;

        constexpr DistinctNumeric(T value)
            : m_value { value }
        {
        }

        /**
         * @return constexpr const T& 
         */
        constexpr const T& value() const 
        { 
            return m_value; 
        }
        
        /**
         * @return constexpr T& 
         */
        constexpr T& value() { return m_value; }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator==(Self const& other) const
        {
            return this->m_value == other.m_value;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator!=(Self const& other) const
        {
            return this->m_value != other.m_value;
        }

        /**
         * @return constexpr Self& 
         */
        constexpr Self& operator++()
        {
            static_assert(Incr, "'++a' is only available for DistinctNumeric types with 'Incr'.");
            this->m_value += 1;
            return *this;
        }

        /**
         * @return constexpr Self 
         */
        constexpr Self operator++(int)
        {
            static_assert(Incr, "'a++' is only available for DistinctNumeric types with 'Incr'.");
            Self ret = this->m_value;
            this->m_value += 1;
            return ret;
        }

        /**
         * @return constexpr Self& 
         */
        constexpr Self& operator--()
        {
            static_assert(Incr, "'--a' is only available for DistinctNumeric types with 'Incr'.");
            this->m_value -= 1;
            return *this;
        }

        /**
         * @return constexpr Self 
         */
        constexpr Self operator--(int)
        {
            static_assert(Incr, "'a--' is only available for DistinctNumeric types with 'Incr'.");
            Self ret = this->m_value;
            this->m_value -= 1;
            return ret;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator>(Self const& other) const
        {
            static_assert(Cmp, "'a>b' is only available for DistinctNumeric types with 'Cmp'.");
            return this->m_value > other.m_value;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator<(Self const& other) const
        {
            static_assert(Cmp, "'a<b' is only available for DistinctNumeric types with 'Cmp'.");
            return this->m_value < other.m_value;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator>=(Self const& other) const
        {
            static_assert(Cmp, "'a>=b' is only available for DistinctNumeric types with 'Cmp'.");
            return this->m_value >= other.m_value;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator<=(Self const& other) const
        {
            static_assert(Cmp, "'a<=b' is only available for DistinctNumeric types with 'Cmp'.");
            return this->m_value <= other.m_value;
        }
        
        /**
         * @return true 
         * @return false 
         */
        constexpr bool operator!() const
        {
            static_assert(Bool, "'!a' is only available for DistinctNumeric types with 'Bool'.");
            return !this->m_value;
        }
        
        /**
         * @return constexpr Self 
         */
        constexpr Self operator~() const
        {
            static_assert(Flags, "'~a' is only available for DistinctNumeric types with 'Flags'.");
            return ~this->m_value;
        }

        /**
         * @param other 
         * @return constexpr Self 
         */
        constexpr Self operator&(Self const& other) const
        {
            static_assert(Flags, "'a&b' is only available for DistinctNumeric types with 'Flags'.");
            return this->m_value & other.m_value;
        }

        /**
         * @param other 
         * @return constexpr Self 
         */
        constexpr Self operator|(Self const& other) const
        {
            static_assert(Flags, "'a|b' is only available for DistinctNumeric types with 'Flags'.");
            return this->m_value | other.m_value;
        }

        /**
         * @param other 
         * @return constexpr Self 
         */
        constexpr Self operator^(Self const& other) const
        {
            static_assert(Flags, "'a^b' is only available for DistinctNumeric types with 'Flags'.");
            return this->m_value ^ other.m_value;
        }

        /**
         * @param other 
         * @return constexpr Self& 
         */
        constexpr Self& operator&=(Self const& other)
        {
            static_assert(Flags, "'a&=b' is only available for DistinctNumeric types with 'Flags'.");
            this->m_value &= other.m_value;
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Self& 
         */
        constexpr Self& operator|=(Self const& other)
        {
            static_assert(Flags, "'a|=b' is only available for DistinctNumeric types with 'Flags'.");
            this->m_value |= other.m_value;
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Self& 
         */
        constexpr Self& operator^=(Self const& other)
        {
            static_assert(Flags, "'a^=b' is only available for DistinctNumeric types with 'Flags'.");
            this->m_value ^= other.m_value;
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Self 
         */
        constexpr Self operator<<(Self const& other) const
        {
            static_assert(Shift, "'a<<b' is only available for DistinctNumeric types with 'Shift'.");
            return this->m_value << other.m_value;
        }

        /**
         * @param other 
         * @return constexpr Self 
         */
        constexpr Self operator>>(Self const& other) const
        {
            static_assert(Shift, "'a>>b' is only available for DistinctNumeric types with 'Shift'.");
            return this->m_value >> other.m_value;
        }

        /**
         * @param other 
         * @return constexpr Self& 
         */
        constexpr Self& operator<<=(Self const& other)
        {
            static_assert(Shift, "'a<<=b' is only available for DistinctNumeric types with 'Shift'.");
            this->m_value <<= other.m_value;
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Self& 
         */
        constexpr Self& operator>>=(Self const& other)
        {
            static_assert(Shift, "'a>>=b' is only available for DistinctNumeric types with 'Shift'.");
            this->m_value >>= other.m_value;
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Self 
         */
        constexpr Self operator+(Self const& other) const
        {
            static_assert(Arith, "'a+b' is only available for DistinctNumeric types with 'Arith'.");
            return this->m_value + other.m_value;
        }

        /**
         * @param other 
         * @return constexpr Self 
         */
        constexpr Self operator-(Self const& other) const
        {
            static_assert(Arith, "'a-b' is only available for DistinctNumeric types with 'Arith'.");
            return this->m_value - other.m_value;
        }

        /**
         * @return constexpr Self 
         */
        constexpr Self operator+() const
        {
            static_assert(Arith, "'+a' is only available for DistinctNumeric types with 'Arith'.");
            return +this->m_value;
        }

        /**
         * @return constexpr Self 
         */
        constexpr Self operator-() const
        {
            static_assert(Arith, "'-a' is only available for DistinctNumeric types with 'Arith'.");
            return -this->m_value;
        }

        /**
         * @param other 
         * @return constexpr Self 
         */
        constexpr Self operator*(Self const& other) const
        {
            static_assert(Arith, "'a*b' is only available for DistinctNumeric types with 'Arith'.");
            return this->m_value * other.m_value;
        }

        /**
         * @param other 
         * @return constexpr Self 
         */
        constexpr Self operator/(Self const& other) const
        {
            static_assert(Arith, "'a/b' is only available for DistinctNumeric types with 'Arith'.");
            return this->m_value / other.m_value;
        }

        /**
         * @param other 
         * @return constexpr Self 
         */
        constexpr Self operator%(Self const& other) const
        {
            static_assert(Arith, "'a%b' is only available for DistinctNumeric types with 'Arith'.");
            return this->m_value % other.m_value;
        }

        /**
         * @param other 
         * @return constexpr Self& 
         */
        constexpr Self& operator+=(Self const& other)
        {
            static_assert(Arith, "'a+=b' is only available for DistinctNumeric types with 'Arith'.");
            this->m_value += other.m_value;
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Self& 
         */
        constexpr Self& operator-=(Self const& other)
        {
            static_assert(Arith, "'a+=b' is only available for DistinctNumeric types with 'Arith'.");
            this->m_value += other.m_value;
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Self& 
         */
        constexpr Self& operator*=(Self const& other)
        {
            static_assert(Arith, "'a*=b' is only available for DistinctNumeric types with 'Arith'.");
            this->m_value *= other.m_value;
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Self& 
         */
        constexpr Self& operator/=(Self const& other)
        {
            static_assert(Arith, "'a/=b' is only available for DistinctNumeric types with 'Arith'.");
            this->m_value /= other.m_value;
            return *this;
        }

        /**
         * @param other 
         * @return constexpr Self& 
         */
        constexpr Self& operator%=(Self const& other)
        {
            static_assert(Arith, "'a%=b' is only available for DistinctNumeric types with 'Arith'.");
            this->m_value %= other.m_value;
            return *this;
        }

    private:
        T m_value {};
    };

    /**
     * @tparam T 
     * @tparam X 
     * @tparam Incr 
     * @tparam Cmp 
     * @tparam Bool 
     * @tparam Flags 
     * @tparam Shift 
     * @tparam Arith 
     */
    template<typename T, typename X, bool Incr, bool Cmp, bool Bool, bool Flags, bool Shift, bool Arith>
    struct Formatter<DistinctNumeric<T, X, Incr, Cmp, Bool, Flags, Shift, Arith>> : Formatter<T> 
    {
        ErrorOr<void> format(FormatBuilder& builder, DistinctNumeric<T, X, Incr, Cmp, Bool, Flags, Shift, Arith> value)
        {
            return Formatter<T>::format(builder, value.value());
        }
    };
} // namespace Mods

#define TYPEDEF_DISTINCT_NUMERIC_GENERAL(T, Incr, Cmp, Bool, Flags, Shift, Arith, NAME) \
    using NAME = DistinctNumeric<T, struct __##NAME##_tag, Incr, Cmp, Bool, Flags, Shift, Arith>;
#define TYPEDEF_DISTINCT_ORDERED_ID(T, NAME) TYPEDEF_DISTINCT_NUMERIC_GENERAL(T, false, true, true, false, false, false, NAME)

/**
 * @tparam T 
 * @tparam X 
 * @tparam Args 
 */
template<typename T, typename X, auto... Args>
struct Traits<Mods::DistinctNumeric<T, X, Args...>> : public GenericTraits<Mods::DistinctNumeric<T, X, Args...>> 
{
    static constexpr bool is_trivial() { return true; }
    static constexpr auto hash(DistinctNumeric<T, X, Args...> const& d) { return Traits<T>::hash(d.value()); }
};

using Mods::DistinctNumeric;
