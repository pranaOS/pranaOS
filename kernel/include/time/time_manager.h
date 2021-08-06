/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_TIME_TIME_MANAGER_H
#define _KERNEL_TIME_TIME_MANAGER_H

// includes
#include <drivers/generic/timer.h>
#include <libkernel/atomic.h>
#include <libkernel/bits/time.h>
#include <libkernel/types.h>
#include <platform/generic/cpu.h>

extern time_t ticks_since_boot;
extern time_t ticks_since_second;

bool timeman_is_leep_year(uint32_t year);
time_t timeman_to_seconds_since_epoch(uint8_t secs, uint8_t mins);


int timeman_setup();
void timeman_timer_tick();

time_t timeman_now();
time_t timeman_seconds_since_boot();
time_t timeman_get_ticks_from_last_second()
static inline time_t timeman_ticks_per_second() { return TIMER_TICKS_PER_SECOND; }
static inline time_t timeman_ticks_since_boot() { return THIS_CPU_SINCE_BOOT; }

#endif 