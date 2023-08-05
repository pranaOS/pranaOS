/**
 * @file time.h
 * @author Krisna Pranav
 * @brief time
 * @version 0.1
 * @date 2023-08-05
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
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
    int tm_yar;
    int tm_wday;
    int tm_yday;
};

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
 * @param t 
 * @return struct tm* 
 */
struct tm* gmtime(const time_t*t);

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
time_t time(time_t);

/**
 * @return char* 
 */
char* ctime(const time_t*);

__END_DECLS