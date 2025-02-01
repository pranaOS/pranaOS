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

        consteval ArbitrarySizedEnum(T v)
            : T(v)
        {}

        constexpr ArbitrarySizedEnum(T v, Badge<ArbitrarySizedEnum<T>>)
            : T(v)
        {}

        template<Integral X>
        [[nodiscard]] consteval ArbitrarySizedEnum<T> operator<<(X other) const
        {
            return T(this->value() << other);
        }

        template<Integral X>
        [[nodiscard]] constexpr bool operator==(X other) const
        {
            return this->value() == T(other);
        }

        [[nodiscard]] constexpr bool operator==(ArbitrarySizedEnum<T> const& other)
        {
            return this->value() == other.value();
        }

        constexpr ArbitrarySizedEnum<T>& operator|=(ArbitrarySizedEnum<T> const& other)
        {
            this->value() |= other.value();
            return *this;
        }
        
        constexpr ArbitrarySizedEnum<T>& operator&=(ArbitrarySizedEnum<T> const& other)
        {
            this->value() &= other.value();
            return *this;
        }

    }; // struct ArbitrarySizedEnum : public T

#define MOD_MAKE_ARBITRARY_SIZED_ENUM(EnumName, T, ...)                               \
    namespace EnumName {                                                             \
    using EnumName = ArbitrarySizedEnum<DistinctNumeric<T, struct __##EnumName##Tag, \
        false, true, false, false, false, false>>;                                   \
    using Type = EnumName;                                                           \
    using UnderlyingType = T;                                                        \
    inline constexpr static EnumName __VA_ARGS__;                                    \
    }

} // namespace Mods