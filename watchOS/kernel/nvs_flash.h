/**
 * @file nvs_flash.h
 * @author Krisna Pranav
 * @brief Data Types
 * @version 6.0
 * @date 2023-06-27
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include "types.h"

esp_err_t nvs_flash_erase();
esp_err_t nvs_flash_init();
