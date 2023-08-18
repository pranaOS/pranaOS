/**
 * @file distinctnums.h
 * @author Krisna Pranav
 * @brief distinctnums
 * @version 6.0
 * @date 2023-07-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>

namespace Mods {

    template<typename T, bool Incr, bool Cmp, bool Bool, bool Flags, bool Shift, bool Arith, typename X>
    class DistinctNumeric {
        using Self = DistinctNumeric<T, Incr, Cmp, Bool, Flags, Shift, Arith, X>;

    public:
        DistinctNumeric(T value)
            : m_value { value }
        { }

        /**
         * @return const T& 
         */
        const T& value() const { 
            return m_value; 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(const Self& other) const {
            return this->m_value == other.m_value;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(const Self& other) const {
            return this->m_value != other.m_value;
        }

        /**
         * @return Self& 
         */
        Self& operator++() {
            static_assert(Incr, "'++a' is only available for type 'Incr'.");
            this->m_value += 1;
            return *this;
        }

        /**
         * @return Self 
         */
        Self operator++(int) {
            static_assert(Incr, "'a++' is only available for type 'Incr'.");
            Self ret = this->m_value;
            this->m_value += 1;
            return ret;
        }

        /**
         * @return Self& 
         */
        Self& operator--() {
            static_assert(Incr, "'--a' is only available for type 'Incr'.");
            this->m_value -= 1;
            return *this;
        }

        /**
         * @return Self 
         */
        Self operator--(int) {
            static_assert(Incr, "'a--' is only available for type 'Incr'.");
            Self ret = this->m_value;
            this->m_value -= 1;
            return ret;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator>(const Self& other) const {
            static_assert(Cmp, "a>b is only available for type 'Cmp'.");
            return this->m_value > other.m_value;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator<(const Self& other) const {
            static_assert(Cmp, "a<b is only available for type 'Cmp'.");
            return this->m_value < other.m_value;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator>=(const Self& other) const {
            static_assert(Cmp, "a>=b is only available for type 'Cmp'.");
            return this->m_value >= other.m_value;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator<=(const Self& other) const{
            static_assert(Cmp, "'a<=b' is only available for types 'Cmp'.");
            return this->m_value <= other.m_value;
        }

        /**
         * @return true 
         * @return false 
         */
        bool operator!() const {
            static_assert(Bool, "'!a' is only available for type 'Bool'.");
            return !this->m_value;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator&&(const Self& other) const {
            static_assert(Bool, "'a&&b' is only available for type 'Bool'.");
            return this->m_value && other.m_value;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator||(const Self& other) const {
            static_assert(Bool, "'a||b' is only available for type 'Bool'.");
            return this->m_value || other.m_value;
        }
        
        /**
         * @return Self 
         */
        Self operator~() const {
            static_assert(Flags, "'~a' is only available for type 'Flags'.");
            return ~this->m_value;
        }

        /**
         * @param other 
         * @return Self 
         */
        Self operator&(const Self& other) const {
            static_assert(Flags, "'a&b' is only available for type 'Flags'.");
            return this->m_value & other.m_value;
        }

        /**
         * @param other 
         * @return Self 
         */
        Self operator|(const Self& other) const{
            static_assert(Flags, "'a|b' is only available for type 'Flags'.");
            return this->m_value | other.m_value;
        }

        /**
         * @param other 
         * @return Self 
         */
        Self operator^(const Self& other) const {
            static_assert(Flags, "'a^b' is only available for type 'Flags'.");
            return this->m_value ^ other.m_value;
        }

        /**
         * @param other 
         * @return Self& 
         */
        Self& operator&=(const Self& other) {
            static_assert(Flags, "'a&=b' is only available for type 'Flags'.");
            this->m_value &= other.m_value;
            return *this;
        }

        /**
         * @param other 
         * @return Self& 
         */
        Self& operator|=(const Self& other) {
            static_assert(Flags, "'a|=b' is only available for type 'Flags'.");
            this->m_value |= other.m_value;
            return *this;
        }

        /**

         * @param other 
         * @return Self& 
         */
        Self& operator^=(const Self& other) {
            static_assert(Flags, "'a^=b' is only available for type 'Flags'.");
            this->m_value ^= other.m_value;
            return *this;
        }

        /**
         * @param other 
         * @return Self 
         */
        Self operator<<(const Self& other) const {
            static_assert(Shift, "'a<<b' is only available for type 'Shift'.");
            return this->m_value << other.m_value;
        }

        /**
         * @param other 
         * @return Self 
         */
        Self operator>>(const Self& other) const {
            static_assert(Shift, "'a>>b' is only available for type 'Shift'.");
            return this->m_value >> other.m_value;
        }

        /**
         * @param other 
         * @return Self& 
         */
        Self& operator<<=(const Self& other) {
            static_assert(Shift, "'a<<=b' is only available for type 'Shift'.");
            this->m_value <<= other.m_value;
            return *this;
        }

        /**
         * @param other 
         * @return Self& 
         */
        Self& operator>>=(const Self& other) {
            static_assert(Shift, "'a>>=b' is only available for type 'Shift'.");
            this->m_value >>= other.m_value;
            return *this;
        }

        /**
         * @param other 
         * @return Self 
         */
        Self operator+(const Self& other) const {
            static_assert(Arith, "'a+b' is only available for type 'Arith'.");
            return this->m_value + other.m_value;
        }

        /**
         * @param other 
         * @return Self 
         */
        Self operator-(const Self& other) const {
            static_assert(Arith, "'a-b' is only available for type 'Arith'.");
            return this->m_value - other.m_value;
        }

        /**
         * @return Self 
         */
        Self operator+() const {
            static_assert(Arith, "'+a' is only available for type 'Arith'.");
            return +this->m_value;
        }

        /**
         * @return Self 
         */
        Self operator-() const {
            static_assert(Arith, "'-a' is only available for type 'Arith'.");
            return -this->m_value;
        }

        /**
         * @param other 
         * @return Self 
         */
        Self operator*(const Self& other) const {
            static_assert(Arith, "'a*b' is only available for type 'Arith'.");
            return this->m_value * other.m_value;
        }

        /**
         * @param other 
         * @return Self 
         */
        Self operator/(const Self& other) const {
            static_assert(Arith, "'a/b' is only available for type 'Arith'.");
            return this->m_value / other.m_value;
        }

        /**
         * @param other 
         * @return Self 
         */
        Self operator%(const Self& other) const {
            static_assert(Arith, "'a%b' is only available for type 'Arith'.");
            return this->m_value % other.m_value;
        }

        /**
         * @param other 
         * @return Self& 
         */
        Self& operator+=(const Self& other) {
            static_assert(Arith, "'a+=b' is only available for type 'Arith'.");
            this->m_value += other.m_value;
            return *this;
        }

        /**
         * @param other 
         * @return Self& 
         */
        Self& operator-=(const Self& other) {
            static_assert(Arith, "'a+=b' is only available for type 'Arith'.");
            this->m_value += other.m_value;
            return *this;
        }
        
        /**
         * @param other 
         * @return Self& 
         */
        Self& operator*=(const Self& other) {
            static_assert(Arith, "'a*=b' is only available for type 'Arith'.");
            this->m_value *= other.m_value;
            return *this;
        }

        /**
         * @param other 
         * @return Self& 
         */
        Self& operator/=(const Self& other) {
            static_assert(Arith, "'a/=b' is only available for type 'Arith'.");
            this->m_value /= other.m_value;
            return *this;
        }

        /**
         * @param other 
         * @return Self& 
         */
        Self& operator%=(const Self& other) {
            static_assert(Arith, "'a%=b' is only available for type 'Arith'.");
            this->m_value %= other.m_value;
            return *this;
        }

    private:
        T m_value;
    };

}

#define TYPEDEF_DISTINCT_NUMERIC_GENERAL(T, Incr, Cmp, Bool, Flags, Shift, Arith, NAME) \
    using NAME = DistinctNumeric<T, Incr, Cmp, Bool, Flags, Shift, Arith, struct __##NAME##_tag>;


#define TYPEDEF_DISTINCT_ORDERED_ID(T, NAME) TYPEDEF_DISTINCT_NUMERIC_GENERAL(T, false, true, true, false, false, false, NAME)

using Mods::DistinctNumeric;
