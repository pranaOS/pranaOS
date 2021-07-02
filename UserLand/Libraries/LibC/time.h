/*
 * Copyright (c) 2021 krishpranav, nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

struct tm {
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

extern long timezone; 
extern long altzone;
extern char* tzname[2];
extern int daylight;

typedef uint32_t clock_t;
typedef int64_t time_t;

struct tm* localtime(const time_t*);
struct tm* gmtime(const time_t*);
time_t mktime(struct tm*);
time_t timegm(struct tm*);
time_t time(time_t*);
char* ctime(const time_t*);
char* ctime_r(const time_t* tm, char* buf);
void tzset();
char* asctime(const struct tm*);
char* asctime_r(const struct tm*, char* buf);

#define CLOCKS_PER_SEC 1000
clock_t clock();

struct timespec {
    time_t tv_sec;
    long tv_nsec;
};

typedef int clockid_t;

#define CLOCK_REALTIME 0
#define CLOCK_MONOTONIC 1
#define CLOCK_MONOTONIC_RAW 4
#define CLOCK_REALTIME_COARSE 5
#define CLOCK_MONOTONIC_COARSE 6
#define TIMER_ABSTIME 99

int clock_gettime(clockid_t, struct timespec*);
int clock_settime(clockid_t, struct timespec*);
int clock_nanosleep(clockid_t, int flags, const struct timespec* requested_sleep, struct timespec* remaining_sleep);
int clock_getres(clockid_t, struct timespec* result);
int nanosleep(const struct timespec* requested_sleep, struct timespec* remaining_sleep);
struct tm* gmtime_r(const time_t* timep, struct tm* result);
struct tm* localtime_r(const time_t* timep, struct tm* result);

double difftime(time_t, time_t);
size_t strftime(char* s, size_t max, const char* format, const struct tm*) __attribute__((format(strftime, 3, 0)));

__END_DECLS
