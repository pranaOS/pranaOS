/**
 * @file TimeUtils.h
 * @author Krisna Pranav
 * @brief Time Utils Header
 * @version 0.1
 * @date 2023-06-16
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, pranaOS Developers
 * 
 */

#pragma once

#include "Config.h"
#include <Arduino.h>

class TimeUtils {
public:
    /**
     * @brief Get the Current Time In Seconds object
     * 
     * @return uint 
     */
    static uint getCurrentTimeInSec();
}