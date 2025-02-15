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
} // namespace Mods::SIMD 