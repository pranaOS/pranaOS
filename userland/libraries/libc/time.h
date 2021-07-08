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

time_t time(time_t *timer);

clock_t clock(void);

struct tm *gmtime(const time_t *timer);

struct tm *localtime(const time_t *timer);

size_t strftime(char *s, size_t n, const char *format, const struct tm *tptr);

time_t mktime(struct tm *ptm);

double difftime(time_t timer2, time_t timer1);

char *asctime(const struct tm *pTime);

char *ctime(const time_t *pTime);

__END_HEADER
