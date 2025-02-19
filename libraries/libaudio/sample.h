/**
 * @file sample.h
 * @author Krisna Pranav
 * @brief sample
 * @version 6.0
 * @date 2025-02-18
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/format.h>
#include <mods/math.h>

namespace Audio
{
constexpr double DYNAMIC_RANGE = 1000;
constexpr double VOLUME_A = 1 / DYNAMIC_RANGE;
double const VOLUME_B = log(DYNAMIC_RANGE);

struct Sample
{
    constexpr Sample() = default;

    /**
     * @param left
     *
     */
    constexpr explicit Sample(double left)
        : left(left), right(left)
    {
    }

    /**
     * @param left
     * @param right
     *
     */
    constexpr Sample(double left, double right)
        : left(left), right(right)
    {
    }
}; // struct Sample

namespace Mods
{
template <>
struct Formatter<Audio::Sample> : Formatter<FormatString>
{
    /**
     * @param builder
     * @param value
     * @return ErrorOr<void>
     */
    ErrorOr<void> format(FormatBuilder& builder, Audio::Sample const& value)
    {
        return Formatter<FormatString>::format(builder, "[{}, {}]", value.left, value.right);
    }
}; // struct Formatter<Audio::Sample> : Formatter<FormatString>
} // namespace Mods
} // namespace Audio