/**
 * @file time.cpp
 * @author Krisna Pranav
 * @brief time
 * @version 6.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <mods/string.h>
#include <mods/string_builder.h>
#include <mods/time.h>
#include <kernel/api/syscall.h>

extern "C" 
{

    /**
     * @param tloc 
     * @return time_t 
     */
    time_t time(time_t* tloc)
    {
        struct timeval tv;
        struct timezone tz;

        if (gettimeofday(&tv, &tz) < 0)
            return (time_t)-1;

        if (tloc)
            *tloc = tv.tv_sec;

        return tv.tv_sec;
    }

    /**
     * @param delta 
     * @param old_delta 
     * @return int 
     */
    int adjtime(const struct timeval* delta, struct timeval* old_delta)
    {
        int rc = syscall(SC_adjtime, delta, old_delta);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param tv 
     * @param __restrict__ 
     * @return int 
     */
    int gettimeofday(struct timeval* __restrict__ tv, void* __restrict__)
    {
        int rc = syscall(SC_gettimeofday, tv);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param tv 
     * @param __restrict__ 
     * @return int 
     */
    int settimeofday(struct timeval* __restrict__ tv, void* __restrict__)
    {
        timespec ts;
        TIMEVAL_TO_TIMESPEC(tv, &ts);
        return clock_settime(CLOCK_REALTIME, &ts);
    }

    /**
     * @param t 
     * @return char* 
     */
    char* ctime(const time_t* t)
    {
        return asctime(localtime(t));
    }

    static const int __seconds_per_day = 60 * 60 * 24;

    /**
     * @param tm 
     * @param t 
     */
    static void time_to_tm(struct tm* tm, time_t t)
    {
        int year = 1970;
        for (; t >= days_in_year(year) * __seconds_per_day; ++year)
            t -= days_in_year(year) * __seconds_per_day;
        for (; t < 0; --year)
            t += days_in_year(year - 1) * __seconds_per_day;
        tm->tm_year = year - 1900;

        ASSERT(t >= 0);
        int days = t / __seconds_per_day;
        tm->tm_yday = days;
        int remaining = t % __seconds_per_day;
        tm->tm_sec = remaining % 60;
        remaining /= 60;
        tm->tm_min = remaining % 60;
        tm->tm_hour = remaining / 60;

        int month;
        for (month = 1; month < 12 && days >= days_in_month(year, month); ++month)
            days -= days_in_month(year, month);

        tm->tm_mday = days + 1;
        tm->tm_wday = day_of_week(year, month, tm->tm_mday);
        tm->tm_mon = month - 1;
    }

    /**
     * @param tm 
     * @param timezone_adjust_seconds 
     * @return time_t 
     */
    static time_t tm_to_time(struct tm* tm, long timezone_adjust_seconds)
    {
        tm->tm_year += tm->tm_mon / 12;
        tm->tm_mon %= 12;

        if (tm->tm_mon < 0) {
            tm->tm_year--;
            tm->tm_mon += 12;
        }

        tm->tm_yday = day_of_year(1900 + tm->tm_year, tm->tm_mon + 1, tm->tm_mday);
        time_t days_since_epoch = years_to_days_since_epoch(1900 + tm->tm_year) + tm->tm_yday;
        auto timestamp = ((days_since_epoch * 24 + tm->tm_hour) * 60 + tm->tm_min) * 60 + tm->tm_sec + timezone_adjust_seconds;
        time_to_tm(tm, timestamp);

        return timestamp;
    }

    /**
     * @param tm 
     * @return time_t 
     */
    time_t mktime(struct tm* tm)
    {
        return tm_to_time(tm, timezone);
    }

    /**
     * @param t 
     * @return struct tm* 
     */
    struct tm* localtime(const time_t* t)
    {
        static struct tm tm_buf;
        return localtime_r(t, &tm_buf);
    }

    /**
     * @param t 
     * @param tm 
     * @return struct tm* 
     */
    struct tm* localtime_r(const time_t* t, struct tm* tm)
    {
        if (!t)
            return nullptr;

        time_to_tm(tm, (*t) - timezone);
        return tm;
    }

    /**
     * @param tm 
     * @return time_t 
     */
    time_t timegm(struct tm* tm)
    {
        return tm_to_time(tm, 0);
    }   

    /**
     * @param t 
     * @return struct tm* 
     */
    struct tm* gmtime(const time_t* t)
    {
        static struct tm tm_buf;
        return gmtime_r(t, &tm_buf);
    }

    /**
     * @param t 
     * @param tm 
     * @return struct tm* 
     */
    struct tm* gmtime_r(const time_t* t, struct tm* tm)
    {
        if (!t)
            return nullptr;

        time_to_tm(tm, *t);
        return tm;
    }

    /**
     * @param tm 
     * @return char* 
     */
    char* asctime(const struct tm* tm)
    {
        static char buffer[69];
        strftime(buffer, sizeof buffer, "%a %b %e %T %Y", tm);
        return buffer;
    }

    /**
     * @param destination 
     * @param max_size 
     * @param format 
     * @param tm 
     * @return size_t 
     */
    size_t strftime(char* destination, size_t max_size, const char* format, const struct tm* tm)
    {
        const char wday_short_names[7][4] = {
            "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
        };
        const char wday_long_names[7][10] = {
            "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
        };
        const char mon_short_names[12][4] = {
            "Jan", "Feb", "Mar", "Apr", "May", "Jun",
            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
        };
        const char mon_long_names[12][10] = {
            "January", "February", "March", "April", "May", "June",
            "July", "August", "September", "October", "November", "December"
        };

        StringBuilder builder { max_size };

        const int format_len = strlen(format);

        for (int i = 0; i < format_len; ++i) {
            if (format[i] != '%') {
                builder.append(format[i]);
            } else {
                if (++i >= format_len)
                    return 0;

                switch (format[i]) {
                case 'a':
                    builder.append(wday_short_names[tm->tm_wday]);
                    break;
                case 'A':
                    builder.append(wday_long_names[tm->tm_wday]);
                    break;
                case 'b':
                    builder.append(mon_short_names[tm->tm_mon]);
                    break;
                case 'B':
                    builder.append(mon_long_names[tm->tm_mon]);
                    break;
                case 'C':
                    builder.appendf("%02d", (tm->tm_year + 1900) / 100);
                    break;
                case 'd':
                    builder.appendf("%02d", tm->tm_mday);
                    break;
                case 'D':
                    builder.appendf("%02d/%02d/%02d", tm->tm_mon + 1, tm->tm_mday, (tm->tm_year + 1900) % 100);
                    break;
                case 'e':
                    builder.appendf("%2d", tm->tm_mday);
                    break;
                case 'h':
                    builder.append(mon_short_names[tm->tm_mon]);
                    break;
                case 'H':
                    builder.appendf("%02d", tm->tm_hour);
                    break;
                case 'I':
                    builder.appendf("%02d", tm->tm_hour % 12);
                    break;
                case 'j':
                    builder.appendf("%03d", tm->tm_yday + 1);
                    break;
                case 'm':
                    builder.appendf("%02d", tm->tm_mon + 1);
                    break;
                case 'M':
                    builder.appendf("%02d", tm->tm_min);
                    break;
                case 'n':
                    builder.append('\n');
                    break;
                case 'p':
                    builder.append(tm->tm_hour < 12 ? "a.m." : "p.m.");
                    break;
                case 'r':
                    builder.appendf("%02d:%02d:%02d %s", tm->tm_hour % 12, tm->tm_min, tm->tm_sec, tm->tm_hour < 12 ? "a.m." : "p.m.");
                    break;
                case 'R':
                    builder.appendf("%02d:%02d", tm->tm_hour, tm->tm_min);
                    break;
                case 'S':
                    builder.appendf("%02d", tm->tm_sec);
                    break;
                case 't':
                    builder.append('\t');
                    break;
                case 'T':
                    builder.appendf("%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);
                    break;
                case 'u':
                    builder.appendf("%d", tm->tm_wday ? tm->tm_wday : 7);
                    break;
                case 'U': {
                    const int wday_of_year_beginning = (tm->tm_wday + 6 * tm->tm_yday) % 7;
                    const int week_number = (tm->tm_yday + wday_of_year_beginning) / 7;
                    builder.appendf("%02d", week_number);
                    break;
                }
                case 'V': {
                    const int wday_of_year_beginning = (tm->tm_wday + 6 + 6 * tm->tm_yday) % 7;
                    int week_number = (tm->tm_yday + wday_of_year_beginning) / 7 + 1;
                    if (wday_of_year_beginning > 3) {
                        if (tm->tm_yday >= 7 - wday_of_year_beginning)
                            --week_number;
                        else {
                            const int days_of_last_year = days_in_year(tm->tm_year + 1900 - 1);
                            const int wday_of_last_year_beginning = (wday_of_year_beginning + 6 * days_of_last_year) % 7;
                            week_number = (days_of_last_year + wday_of_last_year_beginning) / 7 + 1;
                            if (wday_of_last_year_beginning > 3)
                                --week_number;
                        }
                    }
                    builder.appendf("%02d", week_number);
                    break;
                }
                case 'w':
                    builder.appendf("%d", tm->tm_wday);
                    break;
                case 'W': {
                    const int wday_of_year_beginning = (tm->tm_wday + 6 + 6 * tm->tm_yday) % 7;
                    const int week_number = (tm->tm_yday + wday_of_year_beginning) / 7;
                    builder.appendf("%02d", week_number);
                    break;
                }
                case 'y':
                    builder.appendf("%02d", (tm->tm_year + 1900) % 100);
                    break;
                case 'Y':
                    builder.appendf("%d", tm->tm_year + 1900);
                    break;
                case '%':
                    builder.append('%');
                    break;
                default:
                    return 0;
                }
            }
            if (builder.length() + 1 > max_size)
                return 0;
        }

        auto str = builder.build();
        bool fits = str.copy_characters_to_buffer(destination, max_size);
        return fits ? str.length() : 0;
    }

    /// @brief: zones
    long timezone = 0;
    long altzone;
    char* tzname[2];
    int daylight;

    /// @brief: timezone set
    void tzset()
    {
        timezone = 0;
    }

    /**
     * @return clock_t 
     */
    clock_t clock()
    {
        struct tms tms;
        times(&tms);
        return tms.tms_utime + tms.tms_stime;
    }

    /**
     * @param clock_id 
     * @param ts 
     * @return int 
     */
    int clock_gettime(clockid_t clock_id, struct timespec* ts)
    {
        int rc = syscall(SC_clock_gettime, clock_id, ts);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param clock_id 
     * @param ts 
     * @return int 
     */
    int clock_settime(clockid_t clock_id, struct timespec* ts)
    {
        int rc = syscall(SC_clock_settime, clock_id, ts);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param clock_id 
     * @param flags 
     * @param requested_sleep 
     * @param remaining_sleep 
     * @return int 
     */
    int clock_nanosleep(clockid_t clock_id, int flags, const struct timespec* requested_sleep, struct timespec* remaining_sleep)
    {
        Syscall::SC_clock_nanosleep_params params { clock_id, flags, requested_sleep, remaining_sleep };
        int rc = syscall(SC_clock_nanosleep, &params);
        __RETURN_WITH_ERRNO(rc, rc, -1);
    }

    /**
     * @param requested_sleep 
     * @param remaining_sleep 
     * @return int 
     */
    int nanosleep(const struct timespec* requested_sleep, struct timespec* remaining_sleep)
    {
        return clock_nanosleep(CLOCK_REALTIME, 0, requested_sleep, remaining_sleep);
    }

    /**
     * @return int 
     */
    int clock_getres(clockid_t, struct timespec*)
    {
        ASSERT_NOT_REACHED();
    }

    /**
     * @param t1 
     * @param t0 
     * @return double 
     */
    double difftime(time_t t1, time_t t0)
    {
        return (double)(t1 - t0);
    }
}
