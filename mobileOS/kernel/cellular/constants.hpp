/**
 * @file constants.hpp
 * @author Krisna Pranav
 * @brief Cellular Constants
 * @version 6.0
 * @date 2023-06-17
 *
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <chrono>

namespace Cellular
{
/**
 * @brief defaultTimeout
 *
 */
constexpr std::chrono::milliseconds defaultTimeout{5000};

/**
 * @brief longTimeout
 *
 */
constexpr std::chrono::milliseconds defaultLongTimeout{18000};
} // namespace Cellular