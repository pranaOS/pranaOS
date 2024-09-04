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
public:
    /**
     * @brief Construct a new Rtc Date Time object
     * 
     */
    RtcDateTime() {}

    /**
     * @brief Destroy the Rtc Date Time object
     * 
     */
    virtual ~RtcDateTime() {};

    void InitWithUnix32Time(time_t time);

    uint32_t Hour();
    uint32_t Minute();
    uint32_t Second();

    uint32_t DayOfWeek();
    uint32_t Day();
    uint32_t Month();
    uint32_t Year();

private:
    time_t time = 0;
}; // class RtcDateTime