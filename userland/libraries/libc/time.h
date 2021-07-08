/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <__libc__.h>
#include <stddef.h>

__BEGIN_HEADER

#define CLOCKS_PER_SEC 1000

typedef long int time_t;

typedef long int clock_t;

struct tm
{
    int tm_sec;  
    int tm_min;  
    int tm_hour; 
    int tm_mday; 
    int tm_mon;  
    int tm_year; 
    int tm_wday; 
    int tm_yday; 
    int tm_isdst;
};


__END_HEADER 
