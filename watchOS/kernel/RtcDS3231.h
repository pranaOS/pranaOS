/**
 * @file RtcDS3231.h
 * @author Krisna Pranav
 * @brief RTC Ds3231
 * @version 6.0
 * @date 2024-09-04
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <chrono>
#include <ctime>
#include "time.h"
#include "defines.h"

class RtcDateTime
{
private:
    time_t time = 0;
}; // class RtcDateTime