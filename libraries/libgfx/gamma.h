/**
 * @file gamma.h
 * @author Krisna Pranav
 * @brief Gamma
 * @version 6.0
 * @date 2024-10-29
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "color.h"
#include <mods/math.h>
#include <mods/simd.h>

#ifdef __SSE__
#    include <xmmintrin.h>
#endif

#define GAMMA 2.2

namespace Gfx 
{

    using Mods::SIMD::f32x4;

    #ifdef __SSE__

    #    if !defined(__clang__) || __clang_major__ >= 11
    constexpr f32x4 gamma_to_linear4(f32x4 x)
    #    else
    inline f32x4 gamma_to_linear4(f32x4 x)
    #    endif
    {
        return (0.8f + 0.2f * x) * x * x;
    }

    /**
     * @param x 
     * @return f32x4 
     */
    inline f32x4 linear_to_gamma4(f32x4 x)
    {
        constexpr float a = 0.00279491f;
        constexpr float b = 1.15907984f;
        float c = (b / Mods::sqrt(1.0f + a)) - 1;
        return ((b * __builtin_ia32_rsqrtps(x + a)) - c) * x;
    }

    /**
     * @param v1 
     * @param v2 
     * @param mix 
     * @return f32x4 
     */
    inline f32x4 gamma_accurate_lerp4(f32x4 v1, f32x4 v2, float mix)
    {
        return linear_to_gamma4(gamma_to_linear4(v1) * (1 - mix) + gamma_to_linear4(v2) * mix);
    }

    #endif

    /**
     * @param x 
     * @return constexpr float 
     */
    constexpr float gamma_to_linear(float x)
    {
        return (0.8f + 0.2f * x) * x * x;
    }

    /**
     * @param x 
     * @return float 
     */
    inline float linear_to_gamma(float x)
    {
        constexpr float a = 0.00279491;
        constexpr float b = 1.15907984;
        float c = (b / Mods::sqrt(1 + a)) - 1;
        return ((b / Mods::sqrt(x + a)) - c) * x;
    }

    /**
     * @param v1 
     * @param v2 
     * @param mix 
     * @return float 
     */
    inline float gamma_accurate_lerp(float v1, float v2, float mix)
    {
        return linear_to_gamma(gamma_to_linear(v1) * (1 - mix) + gamma_to_linear(v2) * mix);
    }

    /**
     * @param a 
     * @param b 
     * @param mix 
     * @return Color 
     */
    inline Color gamma_accurate_blend(Color a, Color b, float mix)
    {
    #ifdef __SSE__
        f32x4 ac = {
            (float)a.red(),
            (float)a.green(),
            (float)a.blue(),
        };
        f32x4 bc = {
            (float)b.red(),
            (float)b.green(),
            (float)b.blue(),
        };
        f32x4 out = 255.f * gamma_accurate_lerp4(ac * (1.f / 255.f), bc * (1.f / 255.f), mix);
        return Color(out[0], out[1], out[2]);
    #else
        return {
            static_cast<u8>(255.f * gamma_accurate_lerp(a.red() / 255.f, b.red() / 255.f, mix)),
            static_cast<u8>(255.f * gamma_accurate_lerp(a.green() / 255.f, b.green() / 255.f, mix)),
            static_cast<u8>(255.f * gamma_accurate_lerp(a.blue() / 255.f, b.blue() / 255.f, mix)),
        };
    #endif
    }

}
