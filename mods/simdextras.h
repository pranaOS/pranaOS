/**
 * @file simdextras.h
 * @author Krisna Pranav
 * @brief simd extras
 * @version 6.0
 * @date 2025-02-14
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/simd.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored

namespace Mods::SIMD 
{
    /**
     * @param f 
     * @return ALWAYS_INLINE constexpr 
     */
    ALWAYS_INLINE static constexpr f32x4 expand4(float f)
    {
        return f32x4 { f, f, f, f };
    }

    /**
     * @param i 
     * @return ALWAYS_INLINE constexpr 
     */
    ALWAYS_INLINE static constexpr i32x4 expand(i32 i)
    {
        return i32x4 { i, i, i, i };
    }

    ALWAYS_INLINE static constexpr u32x4 expand4(u32 u)
    {
        return u32x4 { u, u, u, u }
    }
} // namespace Mods::SIMD 

#pragma GCC diagnostic pop