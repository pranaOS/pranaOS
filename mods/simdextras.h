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
#pragma GCC diagnostic ignored "-Wpsabi"

namespace Mods::SIMD
{
    /**
     * @param f 
     * @return ALWAYS_INLINE constexpr 
     */
    ALWAYS_INLINE static constexpr f32x4 expand4(float f)
    {
        return f32x4{f, f, f, f};
    }

    /**
     * @param i 
     * @return ALWAYS_INLINE constexpr 
     */
    ALWAYS_INLINE static constexpr i32x4 expand4(i32 i)
    {
        return i32x4{i, i, i, i};
    }

    /**
     * @param u 
     * @return ALWAYS_INLINE constexpr 
     */
    ALWAYS_INLINE static constexpr u32x4 expand4(u32 u)
    {
        return u32x4{u, u, u, u};
    }

    /**
     * @tparam TSrc 
     * @param v 
     * @return ALWAYS_INLINE 
     */
    template <typename TSrc>
    ALWAYS_INLINE static u32x4 to_u32x4(TSrc v)
    {
        return __builtin_convertvector(v, u32x4);
    }

    /**
     * @tparam TSrc 
     * @param v 
     * @return ALWAYS_INLINE 
     */
    template <typename TSrc>
    ALWAYS_INLINE static i32x4 to_i32x4(TSrc v)
    {
        return __builtin_convertvector(v, i32x4);
    }

    /**
     * @tparam TSrc 
     * @param v 
     * @return ALWAYS_INLINE 
     */
    template <typename TSrc>
    ALWAYS_INLINE static f32x4 to_f32x4(TSrc v)
    {
        return __builtin_convertvector(v, f32x4);
    }

    /**
     * @param mask 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE static i32 maskbits(i32x4 mask)
    {
    #if defined(__SSE__)
        return __builtin_ia32_movmskps((f32x4)mask);
    #else
        return ((mask[0] & 0x80000000) >> 31) | ((mask[1] & 0x80000000) >> 30) | ((mask[2] & 0x80000000) >> 29) | ((mask[3] & 0x80000000) >> 28);
    #endif
    }

    /**
     * @param mask 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE static bool all(i32x4 mask)
    {
        return maskbits(mask) == 15;
    }

    /**
     * @param mask 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE static bool any(i32x4 mask)
    {
        return maskbits(mask) != 0;
    }

    /**
     * @param mask 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE static bool none(i32x4 mask)
    {
        return maskbits(mask) == 0;
    }

    /**
     * @param mask 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE static int maskcount(i32x4 mask)
    {
        constexpr static int count_lut[16]{0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};
        return count_lut[maskbits(mask)];
    }

    /**
     * @param a 
     * @param b 
     * @param c 
     * @param d 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE static f32x4 load4(float const* a, float const* b, float const* c, float const* d)
    {
        return f32x4{*a, *b, *c, *d};
    }

    /**
     * @param a 
     * @param b 
     * @param c 
     * @param d 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE static u32x4 load4(u32 const* a, u32 const* b, u32 const* c, u32 const* d)
    {
        return u32x4{*a, *b, *c, *d};
    }

    /**
     * @param a 
     * @param b 
     * @param c 
     * @param d 
     * @param mask 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE static f32x4 load4_masked(float const* a, float const* b, float const* c, float const* d, i32x4 mask)
    {
        int bits = maskbits(mask);
        return f32x4{
            bits & 1 ? *a : 0.f,
            bits & 2 ? *b : 0.f,
            bits & 4 ? *c : 0.f,
            bits & 8 ? *d : 0.f,
        };
    }

    /**
     * @param a 
     * @param b 
     * @param c 
     * @param d 
     * @param mask 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE static i32x4 load4_masked(u8 const* a, u8 const* b, u8 const* c, u8 const* d, i32x4 mask)
    {
        int bits = maskbits(mask);
        return i32x4{
            bits & 1 ? *a : 0,
            bits & 2 ? *b : 0,
            bits & 4 ? *c : 0,
            bits & 8 ? *d : 0,
        };
    }

    /**
     * @param a 
     * @param b 
     * @param c 
     * @param d 
     * @param mask 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE static u32x4 load4_masked(u32 const* a, u32 const* b, u32 const* c, u32 const* d, i32x4 mask)
    {
        int bits = maskbits(mask);
        return u32x4{
            bits & 1 ? *a : 0u,
            bits & 2 ? *b : 0u,
            bits & 4 ? *c : 0u,
            bits & 8 ? *d : 0u,
        };
    }

    /**
     * @tparam VectorType 
     * @tparam UnderlyingType 
     * @param v 
     * @param a 
     * @param b 
     * @param c 
     * @param d 
     * @return ALWAYS_INLINE 
     */
    template <typename VectorType, typename UnderlyingType = decltype(declval<VectorType>()[0])>
    ALWAYS_INLINE static void store4(VectorType v, UnderlyingType* a, UnderlyingType* b, UnderlyingType* c, UnderlyingType* d)
    {
        *a = v[0];
        *b = v[1];
        *c = v[2];
        *d = v[3];
    }

    /**
     * @tparam VectorType 
     * @tparam UnderlyingType 
     * @param v 
     * @param a 
     * @param b 
     * @param c 
     * @param d 
     * @param mask 
     * @return ALWAYS_INLINE 
     */
    template <typename VectorType, typename UnderlyingType = decltype(declval<VectorType>()[0])>
    ALWAYS_INLINE static void store4_masked(VectorType v, UnderlyingType* a, UnderlyingType* b, UnderlyingType* c, UnderlyingType* d, i32x4 mask)
    {
        int bits = maskbits(mask);
        if(bits & 1)
            *a = v[0];
        if(bits & 2)
            *b = v[1];
        if(bits & 4)
            *c = v[2];
        if(bits & 8)
            *d = v[3];
    }

} // namespace Mods::SIMD

#pragma GCC diagnostic pop