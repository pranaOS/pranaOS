/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <sys/types.h>
#include <stdint.h>

/**
 * @brief time spec
 * 
 */
struct timespec {
    time_t tv_sec;  
    long tv_nsec;
};

/**
 * @brief tms
 * 
 */
struct tms {
    clock_t tms_utime;
    clock_t tms_stime;
    clock_t tms_cutime;
    clock_t tms_cstime;
};

/**
 * @brief time value
 * 
 */
struct timeval {
    time_t tv_sec;
    suseconds_t tv_usec;
};

/**
 * @brief Set the boot seconds object
 * 
 * @param bs 
 */
void set_boot_seconds(uint64_t bs);

/**
 * @brief Set the current time object
 * 
 * @param year 
 * @param month 
 * @param day 
 * @param hour 
 * @param minute 
 * @param second 
 */
void set_current_time(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);

/**
 * @brief Get the time object
 * 
 * @param seconds 
 * @return struct time* 
 */
struct time *get_time(int32_t seconds);