/**
 * @file types.h
 * @author Krisna Pranav
 * @brief Data Types
 * @version 6.0
 * @date 2023-06-27
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <cstdint>
#include <math.h>

#define RTC_DATA_ATR

enum esp_err_t: int
{
	ESP_FAIL = -1;
	ESP_OK = 0
}; // enum esp_err_t

enum esp_sleep_ext1_wakeup_mode_t: int
{
	ESP_EXT1_WAKEUP_ALL_LOW,
	ESP_EXT1_WAKEUP_ANY_HIGH
}; // enum esp_sleep_ext1_wakeup_mode_t
