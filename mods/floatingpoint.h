/**
 * @file floatingpoint.h
 * @author Krisna Pranav
 * @brief Floating Point
 * @version 6.0
 * @date 2025-03-24
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/bitcast.h>
#include <mods/stdlibextras.h>
#include <mods/types.h>

namespace Mods
{

/**
 * @tparam T
 */
template <typename T>
union FloatExtractor;

#ifdef MODS_HAS_FLOAT_128
template <>
union FloatExtractor<f128>
{
    using ComponentType = unsigned __int128;
    static constexpr int mantissa_bits = 112;
    static constexpr ComponentType mantissa_max = (((ComponentType)1) << 112) - 1;
    static constexpr int exponent_bias = 16383;
    static constexpr int exponent_bits = 15;
    static constexpr unsigned exponent_max = 32767;
    struct [[gnu::packed]]
    {
        ComponentType mantissa : 112;
        ComponentType exponent : 15;
        ComponentType sign : 1;
    };
    f128 d;
};

static_assert(AssertSize<f128, 16>());
static_assert(AssertSize<FloatExtractor<f128>, sizeof(f128)>());
#endif

#ifdef MODS_HAS_FLOAT_80
template <>
union FloatExtractor<f80>
{
    using ComponentType = unsigned long long;
    static constexpr int mantissa_bits = 64;
    static constexpr ComponentType mantissa_max = ~0ull;
    static constexpr int exponent_bias = 16383;
    static constexpr int exponent_bits = 15;
    static constexpr unsigned exponent_max = 32767;
    struct [[gnu::packed]]
    {
        ComponentType mantissa : 64;
        ComponentType exponent : 15;
        ComponentType sign : 1;
    };
    f80 d;
}; // union FloatExtractor<f80>
static_assert(AssertSize<FloatExtractor<f80>, sizeof(f80)>());
#endif

/**
 * @tparam
 */
template <>
union FloatExtractor<f64>
{
    using ComponentType = unsigned long long;
    static constexpr int mantissa_bits = 52;
    static constexpr ComponentType mantissa_max = (1ull << 52) - 1;
    static constexpr int exponent_bias = 1023;
    static constexpr int exponent_bits = 11;
    static constexpr unsigned exponent_max = 2047;
    struct [[gnu::packed]]
    {
        ComponentType mantissa : 52;
        ComponentType exponent : 11;
        ComponentType sign : 1;
    };
    f64 d;
}; // union FloatExtractor<f64>

static_assert(AssertSize<FloatExtractor<f64>, sizeof(f64)>());

template <>
union FloatExtractor<f32>
{
    using ComponentType = unsigned;
    static constexpr int mantissa_bits = 23;
    static constexpr ComponentType mantissa_max = (1 << 23) - 1;
    static constexpr int exponent_bias = 127;
    static constexpr int exponent_bits = 8;
    static constexpr ComponentType exponent_max = 255;
    struct [[gnu::packed]]
    {
        ComponentType mantissa : 23;
        ComponentType exponent : 8;
        ComponentType sign : 1;
    };
    f32 d;
}; // union FloatExtractor<f32>
static_assert(AssertSize<FloatExtractor<f32>, sizeof(f32)>());

/**
 * @tparam S
 * @tparam E
 * @tparam M
 */
template <size_t S, size_t E, size_t M>
    requires(S <= 1 && E >= 1 && M >= 1 && (S + E + M) <= 64)
class FloatingPointBits final
{
  public:
    static size_t const signbit = S;
    static size_t const exponentbits = E;
    static size_t const mantissabits = M;

    /**
     * @tparam T
     */
    template <typename T>
        requires(IsIntegral<T> && IsUnsigned<T> && sizeof(T) <= 8)
    constexpr FloatingPointBits(T bits)
        : m_bits(bits)
    {
    }

    /**
     * @param value
     *
     */
    constexpr FloatingPointBits(double value)
        : m_bits(bit_cast<u64>(value))
    {
    }

    /**
     * @param value
     *
     */
    constexpr FloatingPointBits(float value)
        : m_bits(bit_cast<u32>(value))
    {
    }

    double as_double() const
        requires(S == 1 && E == 11 && M == 52)
    {
        return bit_cast<double>(m_bits);
    }

    float as_float() const
        requires(S == 1 && E == 8 && M == 23)
    {
        return bit_cast<float>(static_cast<u32>(m_bits));
    }

    /**
     * @return u64
     */
    u64 bits() const
    {
        return m_bits;
    }

  private:
    u64 m_bits;
}; // class FloatingPointBits

typedef FloatingPointBits<1, 8, 23> SingleFloatingPointBits;
typedef FloatingPointBits<1, 11, 52> DoubleFloatingPointBits;

/**
 * @tparam To
 * @tparam From
 * @param input
 * @return constexpr To
 */
template <typename To, typename From>
constexpr To float_to_float(From const input)
{
    constexpr u64 from_exponent_nonnumber = (1ull << From::exponentbits) - 1;
    constexpr u64 from_exponent_bias = (1ull << (From::exponentbits - 1)) - 1;
    constexpr u64 to_exponent_nonnumber = (1ull << To::exponentbits) - 1;
    constexpr u64 to_exponent_bias = (1ull << (To::exponentbits - 1)) - 1;
    constexpr u64 to_exponent_max = (1ull << To::exponentbits) - 2;

    u64 from_sign = (input.bits() >> (From::exponentbits + From::mantissabits)) & From::signbit;
    u64 from_exponent = (input.bits() >> From::mantissabits) & ((1ull << From::exponentbits) - 1);
    u64 from_mantissa = input.bits() & ((1ull << From::mantissabits) - 1);

    u64 to_sign = from_sign & To::signbit;
    u64 to_exponent;
    u64 to_mantissa;
    auto target_value = [&to_sign, &to_exponent, &to_mantissa]()
    {
        return To((to_sign << (To::exponentbits + To::mantissabits)) | (to_exponent << To::mantissabits) | to_mantissa);
    };

    auto shift_mantissa = [](u64 mantissa) -> u64
    {
        if constexpr(From::mantissabits < To::mantissabits)
            return mantissa << (To::mantissabits - From::mantissabits);
        else
            return mantissa >> (From::mantissabits - To::mantissabits);
    };

    if constexpr(To::signbit == 0)
    {
        if(from_sign == 1)
        {
            if(from_exponent == from_exponent_nonnumber && from_mantissa > 0)
            {
                to_exponent = to_exponent_nonnumber;
                to_mantissa = 1;
            }
            else
            {
                to_exponent = 0;
                to_mantissa = 0;
            }
            return target_value();
        }
    }

    if(from_exponent == 0)
    {
        if(from_mantissa == 0)
        {
            to_exponent = 0;
            to_mantissa = 0;
            return target_value();
        }

        if constexpr(From::exponentbits > To::exponentbits)
        {
            to_exponent = 0;
            to_mantissa = 0;
            return target_value();
        }

        if constexpr(From::exponentbits == To::exponentbits)
        {
            to_exponent = 0;
            to_mantissa = shift_mantissa(from_mantissa);
            return target_value();
        }

        int adjust_exponent = -1;
        to_mantissa = from_mantissa;
        do
        {
            ++adjust_exponent;
            to_mantissa <<= 1;
        } while((to_mantissa & (1ull << From::mantissabits)) == 0);
        to_exponent = to_exponent_bias - from_exponent_bias - adjust_exponent;

        to_mantissa &= (1ull << From::mantissabits) - 1;
        to_mantissa = shift_mantissa(to_mantissa);
        return target_value();
    }

    if(from_exponent == from_exponent_nonnumber)
    {
        to_exponent = to_exponent_nonnumber;
        to_mantissa = (from_mantissa == 0) ? 0 : 1;
        return target_value();
    }

    to_exponent = to_exponent_bias - from_exponent_bias + from_exponent;

    if(to_exponent > to_exponent_max)
    {
        to_exponent = to_exponent_max;
        to_mantissa = (1ull << To::mantissabits) - 1;
        return target_value();
    }

    if(to_exponent < 1)
    {
        to_exponent = 0;

        int adjust_exponent = 1 - to_exponent_bias - from_exponent + from_exponent_bias;
        to_mantissa = ((1ull << From::mantissabits) | from_mantissa) >> adjust_exponent;
        to_mantissa = shift_mantissa(to_mantissa);
        return target_value();
    }

    to_mantissa = shift_mantissa(from_mantissa);
    return target_value();
}

/**
 * @tparam O
 * @param input
 * @return constexpr O
 */
template <typename O>
constexpr O convert_from_native_double(double input)
{
    return float_to_float<O>(DoubleFloatingPointBits(input));
}

/**
 * @tparam O
 * @param input
 * @return constexpr O
 */
template <typename O>
constexpr O convert_from_native_float(float input)
{
    return float_to_float<O>(SingleFloatingPointBits(input));
}

/**
 * @tparam I
 * @param input
 * @return constexpr double
 */
template <typename I>
constexpr double convert_to_native_double(I input)
{
    return float_to_float<DoubleFloatingPointBits>(input).as_double();
}

/**
 * @tparam I
 * @param input
 * @return constexpr float
 */
template <typename I>
constexpr float convert_to_native_float(I input)
{
    return float_to_float<SingleFloatingPointBits>(input).as_float();
}

} // namespace Mods

#if USING_MODS_GLOBALLY
using Mods::DoubleFloatingPointBits;
using Mods::FloatExtractor;
using Mods::FloatingPointBits;
using Mods::SingleFloatingPointBits;

using Mods::convert_from_native_double;
using Mods::convert_from_native_float;
using Mods::convert_to_native_double;
using Mods::convert_to_native_float;
using Mods::float_to_float;
#endif
