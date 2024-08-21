/**
 * @file time.h
 * @author Krisna Pranav
 * @brief time
 * @version 0.1
 * @date 2023-08-05
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

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
}; // struct tm

extern long timezone; 
extern long altzone;
extern char* tzname[2];
extern int daylight;

typedef uint32_t clock_t;
typedef int64_t time_t;

/**
 * @return struct tm* 
 */
struct tm* localtime(const time_t*);

/**
 * @return struct tm* 
 */
struct tm* gmtime(const time_t*);

/**
 * @return time_t 
 */
time_t mktime(struct tm*);

/**
 * @return time_t 
 */
time_t timegm(struct tm*);

/**
 * @return time_t 
 */
time_t time(time_t*);

/**
 * @return char* 
 */
char* ctime(const time_t*);

void tzset();

/**
 * @return char* 
 */
char* asctime(const struct tm*);

#define CLOCKS_PER_SEC 1000

/**
 * @return clock_t 
 */
clock_t clock();

struct timespec 
{
    time_t tv_sec;
    long tv_nsec;
}; // struct timespec

typedef int clockid_t;

#define CLOCK_REALTIME 0
#define CLOCK_MONOTONIC 1
#define TIMER_ABSTIME 99

/**
 * @return int 
 */
int clock_gettime(clockid_t, struct timespec*);

/**
 * @return int 
 */
int clock_settime(clockid_t, struct timespec*);

/**
 * @param flags 
 * @param requested_sleep 
 * @param remaining_sleep 
 * @return int 
 */
int clock_nanosleep(clockid_t, int flags, const struct timespec* requested_sleep, struct timespec* remaining_sleep);

/**
 * @param result 
 * @return int 
 */
int clock_getres(clockid_t, struct timespec* result);

/**
 * @param requested_sleep 
 * @param remaining_sleep 
 * @return int 
 */
int nanosleep(const struct timespec* requested_sleep, struct timespec* remaining_sleep);

/**
 * @param timep 
 * @param result 
 * @return struct tm* 
 */
struct tm* gmtime_r(const time_t* timep, struct tm* result);

/**
 * @param timep 
 * @param result 
 * @return struct tm* 
 */
struct tm* localtime_r(const time_t* timep, struct tm* result);

/**
 * @return double 
 */
double difftime(time_t, time_t);

/**
 * @param s 
 * @param max 
 * @param format 
 * @return size_t 
 */
size_t strftime(char* s, size_t max, const char* format, const struct tm*);

__END_DECLS
