/**
 * @file rtc.h
 * @author Krisna Pranav
 * @brief rtc
 * @version 6.0
 * @date 2023-09-13
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/unixtypes.h>

namespace RTC 
{
    void initialize();

    /**
     * @return time_t 
     */
    time_t now();
    time_t boot_time();

    /**
     * @param year 
     * @param month 
     * @param day 
     * @param hour 
     * @param minute 
     * @param second 
     */
    void read_registers(unsigned& year, unsigned& month, unsigned& day, unsigned& hour, unsigned& minute, unsigned& second);
} // namespace RTC
