/**
 * @file enumbits.h
 * @author Krisna Pranav
 * @brief enum bits
 * @version 6.0
 * @date 2024-09-02
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "mods/stdlibextra.h"

#define MOD_ENUM_BITWISE_OPERATORS(Enum) \
    _MOD_ENUM_BITWISE_OPERATORS_INTERNAL(Enum, )

#define MOD_ENUM_BITWISE_FRIEND_OPERATORS(Enum) \
    _MOD_ENUM_BITWISE_OPERATORS_INTERNAL(Enum, friend)

#define _MOD_ENUM_BITWISE_OPERATORS_INTERNAL(Enum, Prefix)                  \
                                                                           \
    [[nodiscard]] Prefix constexpr Enum operator|(Enum lhs, Enum rhs)      \
    {                                                                      \
        using Type = UnderlyingType<Enum>;                                 \
        return static_cast<Enum>(                                          \
            static_cast<Type>(lhs) | static_cast<Type>(rhs));              \
    }                                                                      \
                                                                           \
    [[nodiscard]] Prefix constexpr Enum operator&(Enum lhs, Enum rhs)      \
    {                                                                      \
        using Type = UnderlyingType<Enum>;                                 \
        return static_cast<Enum>(                                          \
            static_cast<Type>(lhs) & static_cast<Type>(rhs));              \
    }                                                                      \
                                                                           \
    [[nodiscard]] Prefix constexpr Enum operator^(Enum lhs, Enum rhs)      \
    {                                                                      \
        using Type = UnderlyingType<Enum>;                                 \
        return static_cast<Enum>(                                          \
            static_cast<Type>(lhs) ^ static_cast<Type>(rhs));              \
    }                                                                      \
                                                                           \
    [[nodiscard]] Prefix constexpr Enum operator~(Enum rhs)                \
    {                                                                      \
        using Type = UnderlyingType<Enum>;                                 \
        return static_cast<Enum>(                                          \
            ~static_cast<Type>(rhs));                                      \
    }                                                                      \
                                                                           \
    Prefix constexpr Enum& operator|=(Enum& lhs, Enum rhs)                 \
    {                                                                      \
        using Type = UnderlyingType<Enum>;                                 \
        lhs = static_cast<Enum>(                                           \
            static_cast<Type>(lhs) | static_cast<Type>(rhs));              \
        return lhs;                                                        \
    }                                                                      \
                                                                           \
    Prefix constexpr Enum& operator&=(Enum& lhs, Enum rhs)                 \
    {                                                                      \
        using Type = UnderlyingType<Enum>;                                 \
        lhs = static_cast<Enum>(                                           \
            static_cast<Type>(lhs) & static_cast<Type>(rhs));              \
        return lhs;                                                        \
    }                                                                      \
                                                                           \
    Prefix constexpr Enum& operator^=(Enum& lhs, Enum rhs)                 \
    {                                                                      \
        using Type = UnderlyingType<Enum>;                                 \
        lhs = static_cast<Enum>(                                           \
            static_cast<Type>(lhs) ^ static_cast<Type>(rhs));              \
        return lhs;                                                        \
    }                                                                      \
                                                                           \
    Prefix constexpr bool has_flag(Enum value, Enum mask)                  \
    {                                                                      \
        using Type = UnderlyingType<Enum>;                                 \
        return static_cast<Type>(value & mask) == static_cast<Type>(mask); \
    }                                                                      \
                                                                           \
    Prefix constexpr bool has_any_flag(Enum value, Enum mask)              \
    {                                                                      \
        using Type = UnderlyingType<Enum>;                                 \
        return static_cast<Type>(value & mask) != 0;                       \
    }
