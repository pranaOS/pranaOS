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
#include <mods/simd.h>
#include <mods/simdextras.h>
#include <math.h>

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
    ALWAYS_INLINE static f32x4 ceil_int_range(f32x4 v)
    {
        auto t = truncate_int_range(v);
        return t < v ? t + 1.0f : t;
    }

    /**
     * @param v 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE static f32x4 frac_int_range(f32x4 v)
    {
        return v - floor_int_range(v);
    }

    /**
     * @param v 
     * @param min 
     * @param max 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE static f32x4 clamp(f32x4 v, f32x4 min, f32x4 max)
    {
        return v < min ? min : (v > max ? max : v);
    }   

    /**
     * @param v 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE static f32x4 exp(f32x4 v)
    {
        return f32x4{
            expf(v[0]),
            expf(v[1]),
            expf(v[2]),
            expf(v[3]),
        };
    }

    /**
     * @param v 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE static f32x4 sqrt(f32x4 v)
    {
    #ifdef __SSE__
        return __builtin_ia32_sqrtps(v);
    #else
        return f32x4{
            Mods::sqrt(v[0]),
            Mods::sqrt(v[1]),
            Mods::sqrt(v[2]),
            Mods::sqrt(v[3]),
        };
    #endif
    }

    /**
     * @param v 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE static f32x4 rsqrt(f32x4 v)
    {
    #ifdef __SSE__
        return __builtin_ia32_rsqrtps(v);
    #else
        return f32x4{
            1.f / Mods::sqrt(v[0]),
            1.f / Mods::sqrt(v[1]),
            1.f / Mods::sqrt(v[2]),
            1.f / Mods::sqrt(v[3]),
        };
    #endif
    }
} // namespace Mods::SIMD

#pragma GCC diagnostic pop