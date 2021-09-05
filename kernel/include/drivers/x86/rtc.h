/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once 

#include <libkern/types.h>

void rtc_load_time(uint8_t* secs, uint8_t* mins, uint8_t* hrs, uint8_t* day, uint8_t* month, uint32_t* year);
