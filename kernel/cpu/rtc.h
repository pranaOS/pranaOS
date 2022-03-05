/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <stdint.h>

/**
 * @brief rtc[get datetime]
 * 
 * @param year 
 * @param month 
 * @param day 
 * @param hour 
 * @param minute 
 * @param second 
 */
void rtc_get_datetime(uint16_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute, uint8_t *second);

/**
 * @brief init function
 * 
 */
void rtc_init();