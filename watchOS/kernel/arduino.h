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
 * @brief 
 * 
 */
void delay(long millis);

/**
 * @param a 
 * @param b 
 * @return int32_t 
 */
int32_t min(int32_t a, int32_t b);

/**
 * @param a 
 * @param b 
 * @return int32_t 
 */
int32_t max(int32_t a, int32_t b);
