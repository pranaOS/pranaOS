/**
 * @file batteryutils.h
 * @author Krisna Pranav
 * @brief batteryutils
 * @version 6.0
 * @date 2025-07-03
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <config.h>

namespace LibUtils
{

    class BatteryUtils
    {
    public:
      int getBatteryPercentage();
      int getCycleCounts();

      BatteryUtils() {};
      ~BatteryUtils() {};
    }; // class BatteryUtils

} // namespace LibUtils