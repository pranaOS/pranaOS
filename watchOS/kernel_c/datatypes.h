/**
 * @file datatypes.h
 * @author Krisna Pranav
 * @brief Data Types
 * @version 6.0
 * @date 2023-06-27
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <cstdint>
#include <math.h>

enum esp_err_t: int
{
	ESP_FAIL = -1,
	ESP_OK = 0
}; // enum esp_err_t

enum esp_sleep_ext1_wakeup_mode_t: int
{
	ESP_EXT1_WAKEUP_ALL_LOW,
	ESP_EXT1_WAKEUP_ANY_HIGH,
}; // esp_sleep_ext1_wakeup_mode_t

enum esp_sleep_source_t
{
	ESP_SLEEP_WAKEUP_UNDEFINED,
	ESP_SLEEP_WAKEUP_TIMER
}; // enum esp_sleep_source_t

typedef esp_sleep_source_t esp_sleep_wakeup_cause_t;
