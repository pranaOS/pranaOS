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

    }; // struct Sample
} // namespace Audio