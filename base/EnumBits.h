/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include "base/StdLibExtras.h"


#define BASE_ENUM_BITWISE_OPERATORS(Enum) \
        _BASE_ENUM_BITWISE_OPERATOR_INTERNAL(Enum, )

#define BASE_ENUM_BITWISE_FRIEND_OPERATORS(Enum) \
    _BASE_ENUM_BITWISE_OPERATORS_INTERNAL(Enum, friend)


#define _BASE_ENUM_BITWISE_OPERATORS_INTERNAL(Enum, Prefix)                  \
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