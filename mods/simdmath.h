/**
 * @file simdmath.h
 * @author Krisna Pranav
 * @brief simd math
 * @version 6.0
 * @date 2025-02-15
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#ifndef __SSE__
#include <mods/math.h>
#endif

#include <mods/math.h>
#include <mods/simd.h>
#include <mods/simdextras.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpsabi"

namespace Mods::SIMD 
{
    /**
     * @param v 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE static f32x4 truncate_int_range(f32x4 v)
    {
        return to_f32x4(to_i32x4(v));
    }

    /**
     * @param v 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE static f32x4 floor_int_range(f32x4 v)
    {
        auto t = truncate_int_range(v);
        return t > v ? t - 1.0f : t;
    }

    /**
     * @param v 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE static f32x4 ciel_int_range(f32x4 v)
    {
        auto t = truncate_int_range(v);
        return t < v ? t + 1.0f : t;
    }

    ALWAYS_INLINE static f32x4 clamp(f32x4 v, f32x4 min, f32x4 max)
    {   
        return v < min ? min : (v > max ? max : v);
    }
} // namespace Mods::SIMD 