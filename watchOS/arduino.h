/**
 * @file arduino.h
 * @author Krisna Pranav
 * @brief Arduino
 * @version 6.0
 * @date 2023-06-27
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include "types.h"
#include "defines.h"

unsigned long millis();

long random(int howbig);

long random(int howsmall, int howbig);

void delay(long millis);
