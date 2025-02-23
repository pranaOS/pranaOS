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

#include <kernel/api/posix/time.h>

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
}; // struct tm

extern long timezone; 
extern long altzone;
extern char* tzname[2];
extern int daylight;

typedef uint32_t clock_t;
typedef int64_t time_t;

struct tm* localtime(time_t const*);
struct tm* gmtime(time_t const*);
time_t mktime(struct tm*);
time_t timegm(struct tm*);
time_t time(time_t*);
char* ctime(time_t const*);

/**
 * @param tm 
 * @param buf 
 * @return char* 
 */
char* ctime_r(time_t const* tm, char* buf);

void tzset(void);

/**
 * @return char* 
 */
char* asctime(const struct tm*);

/**
 * @param buf 
 * @return char* 
 */
char* asctime_r(const struct tm*, char* buf);

clock_t clock(void);

int clock_gettime(clockid_t, struct timespec*);
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
struct tm* gmtime_r(time_t const* timep, struct tm* result);

/**
 * @param timep 
 * @param result 
 * @return struct tm* 
 */
struct tm* localtime_r(time_t const* timep, struct tm* result);

double difftime(time_t, time_t);

size_t strftime(char* s, size_t max, char const* format, const struct tm*) __attribute__((format(strftime, 3, 0)));

__END_DECLS
