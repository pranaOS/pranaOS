/**
 * @file floatingpointstringconversions.h
 * @author Krisna Pranav
 * @brief Floating Point String Conversions
 * @version 6.0
 * @date 2025-03-24
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#ifdef KERNEL
#error This file should not be included in the KERNEL as it deals with doubles \
           and there is no guraantee does not do any floating point computations.
#endif

#include <mods/stringview.h>

namespace Mods
{

static constexpr char floating_point_decimal_separator = '.';

enum class FloatingPointError
{
    None,
    NoOrInvalidInput,
    OutOfRange,
    RoundedDownToZero
}; // enum class FloatingPointError

/**
 * @tparam T
 */
template <FloatingPoint T>
struct FloatingPointParseResults
{
    char const* end_ptr{nullptr};
    FloatingPointError error = FloatingPointError::None;
    T value{};

    [[nodiscard]] bool parsed_value() const
    {
        return error != FloatingPointError::NoOrInvalidInput;
    }
}; // struct FloatingPointParseResults

/**
 * @tparam T
 * @param start
 * @param end
 * @return FloatingPointParseResults<T>
 */
template <FloatingPoint T = double>
FloatingPointParseResults<T> parse_first_floating_point(char const* start, char const* end);

/**
 * @tparam T
 * @param start
 * @return FloatingPointParseResults<T>
 */
template <FloatingPoint T = double>
FloatingPointParseResults<T> parse_first_floating_point_until_zero_character(char const* start);

/**
 * @tparam T
 * @param start
 * @param end
 * @return Optional<T>
 */
template <FloatingPoint T = double>
Optional<T> parse_floating_point_completely(char const* start, char const* end);

/**
 * @tparam T
 * @param start
 * @return FloatingPointParseResults<T>
 */
template <FloatingPoint T = double>
FloatingPointParseResults<T> parse_first_hexfloat_until_zero_character(char const* start);

} // namespace Mods

#if USING_MODS_GLOBALLY
using Mods::parse_first_floating_point;
using Mods::parse_first_hexfloat_until_zero_character;
using Mods::parse_floating_point_completely;
#endif
