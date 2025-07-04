/**
 * @file batteryutils.h
 * @author Krisna Pranav
 * @brief Advanced battery utility functions
 * @version 7.0
 * @date 2024-05-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <config.h>
#include <cstdint>

namespace LibUtils
{
    /**
     * @brief Represents the charging state of the battery.
     */
    enum class BatteryChargingState : uint8_t
    {
        Unknown,
        Discharging,
        Charging,
        Full
    };

    /**
    * @brief A collection of utility functions to query battery status.
    *
    * This class provides a static interface, so it does not need to be instantiated.
    */

} // namespace LibUtils