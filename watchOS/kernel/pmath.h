/**
 * @file pmath.h
 * @author Krisna Pranav
 * @brief PMath
 * @version 6.0
 * @date 2023-07-04
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <Arduino.h>

float lon2itlex(float lon, uint8_t z);

float getTileResolution(float lat, uint8_t z);
