/**
 * @file valuescaller.hpp
 * @author Krisna Pranav
 * @brief ValueScaller
 * @version 6.0
 * @date 2023-06-17
 *
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <optional>

namespace Utils
{

/**
 * @brief Range
 *
 * @tparam T
 */
template <typename T>
struct Range
{
    T min;
    T max;
};

/**
 * @brief scaleValue
 *
 * @tparam T
 * @param inputRange
 * @param outputRange
 * @param input
 * @return std::optional<T>
 */
template <typename T>
std::optional<T> scaleValue(const Range<T> inputRange, const Range<T> outputRange, const T input)
{
    if(input > inputRange.max || input < inputRange.min)
    {
        return {};
    }

    const auto inRangeVal = inputRange.max - inputRange.min;
    const auto outRangeVal = outputRange.max - outputRange.min;

    if(outRangeVal == 0 || inRangeVal == 0)
    {
        return outputRange.min;
    }
    float slope = 1.0 * (outRangeVal) / (inRangeVal);
    auto output = outputRange.min + slope * (input - inputRange.min);

    return static_cast<T>(std::floor(output));
}

/**
 * @brief Entry
 *
 * @tparam T
 */
template <typename T>
struct Entry
{
    const Range<T> input;
    const Range<T> output;
};

/**
 * @brief findAndScaleValue
 *
 * @tparam T
 * @tparam N
 * @param entries
 * @param val
 * @return std::optional<T>
 */
template <typename T, const size_t N>
[[nodiscard]] std::optional<T> findAndScaleValue(const std::array<Entry<T>, N>& entries, const T val)
{
    auto result = std::find_if(entries.begin(), entries.end(), [val](const auto& e)
                               { return val >= e.input.min && val <= e.input.max; });

    if(result != entries.end())
    {
        return scale_value(result->input, result->output, val);
    }

    return {};
}
} // namespace Utils