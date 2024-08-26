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

/**
 * @param howbig 
 * @return long 
 */
long random(int howbig);

/**
 * @param howsmall 
 * @param howbig 
 * @return long 
 */
long random(int howsmall, int howbig);

/**
 * @param millis 
 */
void delay(long millis);
