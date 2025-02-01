/**
 * @file arbitrarysizedenum.h
 * @author Krisna Pranav
 * @brief Arbitrary Sized Enum
 * @version 6.0
 * @date 2025-02-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/distinctnums.h>

namespace Mods 
{
    /**
     * @tparam T 
     */
    template<typename T>
    struct ArbitrarySizedEnum : public T 
    {
        using T::T;

        /**
         * @param v 
         * @return consteval 
         */
        consteval ArbitrarySizedEnum(T v)
            : T(v)
        {
        }

        constexpr ArbitrarySizedEnum(T v, Badge<ArbitrarySizedEnum<T>>)
            : T(v)
        {
        }

        /**
         * @tparam X 
         * @param other 
         * @return consteval 
         */
        template<Integral X>
        [[nodiscard]] consteval ArbitrarySizedEnum<T> operator<<(X other) const
        {
            return T(this->value() << other);
        }   

        /**
         * @tparam X 
         * @param other 
         * @return constexpr ArbitrarySizedEnum<T>& 
         */
        template<Integral X>
        constexpr ArbitrarySizedEnum<T>& operator<<=(X other)
        {
            this->value() <<= other;
            return *this;
        }

        /**
         * @tparam X 
         * @param other 
         * @return consteval 
         */
        template<Integral X>
        [[nodiscard]] consteval ArbitrarySizedEnum<T> operator>>(X other) const
        {
            return T(this->value() >> other);
        }

        /**
         * @tparam X 
         * @param other 
         * @return constexpr ArbitrarySizedEnum<T>& 
         */
        template<Integral X>
        constexpr ArbitrarySizedEnum<T>& operator>>=(X other)
        {
            this->value() >>= other;
            return *this;
        }

        /**
         * @tparam X 
         * @param other 
         * @return true 
         * @return false 
         */
        template<Integral X>
        [[nodiscard]] constexpr bool operator==(X other) const
        {
            return this->value() == T(other);
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        [[nodiscard]] constexpr bool operator==(ArbitrarySizedEnum<T> const& other) const
        {
            return this->value() == other.value();
        }

        /**
         * @param other 
         * @return constexpr ArbitrarySizedEnum<T> 
         */
        [[nodiscard]] constexpr ArbitrarySizedEnum<T> operator|(ArbitrarySizedEnum<T> const& other) const
        {
            return { T(this->value() | other.value()), {} };
        }

        /**
         * @param other 
         * @return constexpr ArbitrarySizedEnum<T> 
         */
        [[nodiscard]] constexpr ArbitrarySizedEnum<T> operator&(ArbitrarySizedEnum<T> const& other) const
        {
            return { T(this->value() & other.value()), {} };
        }

        /**
         * @param other 
         * @return constexpr ArbitrarySizedEnum<T> 
         */
        [[nodiscard]] constexpr ArbitrarySizedEnum<T> operator^(ArbitrarySizedEnum<T> const& other) const
        {
            return { T(this->value() ^ other.value()), {} };
        }

        /**
         * @return constexpr ArbitrarySizedEnum<T> 
         */
        [[nodiscard]] constexpr ArbitrarySizedEnum<T> operator~() const
        {
            return { T(~this->value()), {} };
        }

        /**
         * @param other 
         * @return constexpr ArbitrarySizedEnum<T>& 
         */
        constexpr ArbitrarySizedEnum<T>& operator|=(ArbitrarySizedEnum<T> const& other)
        {
            this->value() |= other.value();
            return *this;
        }

        /**
         * @param other 
         * @return constexpr ArbitrarySizedEnum<T>& 
         */
        constexpr ArbitrarySizedEnum<T>& operator&=(ArbitrarySizedEnum<T> const& other)
        {
            this->value() &= other.value();
            return *this;
        }

        /**
         * @param other 
         * @return constexpr ArbitrarySizedEnum<T>& 
         */
        constexpr ArbitrarySizedEnum<T>& operator^=(ArbitrarySizedEnum<T> const& other)
        {
            this->value() ^= other.value();
            return *this;
        }

        /**
         * @param mask 
         * @return true 
         * @return false 
         */
        [[nodiscard]] constexpr bool has_flag(ArbitrarySizedEnum<T> const& mask) const
        {
            return (*this & mask) == mask;
        }

        /**
         * @param mask 
         * @return true 
         * @return false 
         */
        [[nodiscard]] constexpr bool has_any_flag(ArbitrarySizedEnum<T> const& mask) const
        {
            return (*this & mask) != 0u;
        }
    }; // struct ArbitrarySizedEnum : public T

#define MODS_MAKE_ARBITRARY_SIZED_ENUM(EnumName, T, ...)                               \
    namespace EnumName {                                                               \
    using EnumName = ArbitrarySizedEnum<DistinctNumeric<T, struct __##EnumName##Tag,   \
        false, true, false, false, false, false>>;                                     \
    using Type = EnumName;                                                             \
    using UnderlyingType = T;                                                          \
    inline constexpr static EnumName __VA_ARGS__;                                      \
    }

}

using Mods::ArbitrarySizedEnum;
