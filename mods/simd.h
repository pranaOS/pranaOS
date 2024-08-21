/**
 * @file simd.h
 * @author Krisna Pranav
 * @brief SIMD
 * @version 6.0
 * @date 2023-07-25
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "types.h"

namespace Mods::SIMD 
{
    /// @brief: i8
    using i8x2 = i8 __attribute__((vector_size(2)));
    using i8x4 = i8 __attribute__((vector_size(4)));
    using i8x8 = i8 __attribute__((vector_size(8)));
    using i8x16 = i8 __attribute__((vector_size(16)));
    using i8x32 = i8 __attribute__((vector_size(32)));

    /// @brief: i16
    using i16x2 = i16 __attribute__((vector_size(4)));
    using i16x4 = i16 __attribute__((vector_size(8)));
    using i16x8 = i16 __attribute__((vector_size(16)));
    using i16x16 = i16 __attribute__((vector_size(32)));

    /// @brief: i32
    using i32x2 = i32 __attribute__((vector_size(8)));
    using i32x4 = i32 __attribute__((vector_size(16)));
    using i32x8 = i32 __attribute__((vector_size(32)));

    /// @brief: i64
    using i64x2 = i64 __attribute__((vector_size(16)));
    using i64x4 = i64 __attribute__((vector_size(32)));

    /// @brief: u8
    using u8x2 = u8 __attribute__((vector_size(2)));
    using u8x4 = u8 __attribute__((vector_size(4)));
    using u8x8 = u8 __attribute__((vector_size(8)));
    using u8x16 = u8 __attribute__((vector_size(16)));
    using u8x32 = u8 __attribute__((vector_size(32)));

    /// @brief: u16
    using u16x2 = u16 __attribute__((vector_size(4)));
    using u16x4 = u16 __attribute__((vector_size(8)));
    using u16x8 = u16 __attribute__((vector_size(16)));
    using u16x16 = u16 __attribute__((vector_size(32)));

    /// @brief: u32
    using u32x2 = u32 __attribute__((vector_size(8)));
    using u32x4 = u32 __attribute__((vector_size(16)));
    using u32x8 = u32 __attribute__((vector_size(32)));

    /// @brief: u64
    using u64x2 = u64 __attribute__((vector_size(16)));
    using u64x4 = u64 __attribute__((vector_size(32)));

    /// @brief: f32
    using f32x2 = float __attribute__((vector_size(8)));
    using f32x4 = float __attribute__((vector_size(16)));
    using f32x8 = float __attribute__((vector_size(32)));

    /// @brief f64
    using f64x2 = double __attribute__((vector_size(16)));
    using f64x4 = double __attribute__((vector_size(32)));

} // namespace Mods::SIMD
