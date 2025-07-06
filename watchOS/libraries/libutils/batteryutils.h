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
    class BatteryUtils
    {
    public:
        // Make the class non-instantiable, as it only contains static utilities.
        BatteryUtils() = delete;
        ~BatteryUtils() = delete;
        BatteryUtils(const BatteryUtils&) = delete;
        BatteryUtils& operator=(const BatteryUtils&) = delete;

        /**
         * @brief Gets the current battery level as a percentage.
         * @return int The battery percentage (0-100), or a negative value on error.
         */
        static int getBatteryPercentage() noexcept;

        /**
         * @brief Gets the number of battery charge/discharge cycles.
         * @return int The number of cycles, or a negative value on error.
         */
        static int getCycleCount() noexcept;

        /**
         * @brief Gets the current battery voltage in millivolts (mV).
         * @return int The voltage in mV, or a negative value on error.
         */
        static int getVoltage() noexcept;

        /**
         * @brief Gets the current charging state of the battery.
         * @return BatteryChargingState The current state.
         */
        static BatteryChargingState getChargingState() noexcept;
    }; // class BatteryUtils

} // namespace LibUtils