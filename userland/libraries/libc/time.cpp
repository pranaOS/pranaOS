/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <assert.h>
#include <time.h>
#include <pranaos/Time.h>

time_t time(time_t *timer)
{
    time_t now = (time_t)timestamp_now();
    *timer = now;
    return now;
}

clock_t clock(void)
{
    return 1;
}