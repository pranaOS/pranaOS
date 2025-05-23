/**
 * @file time.cpp
 * @author Krisna Pranav
 * @brief time lib
 * @version 6.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/dateconstants.h>
#include <mods/string.h>
#include <mods/stringbuilder.h>
#include <mods/time.h>
#include <kernel/api/timepage.h>
#include <libtimezone/timezone.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/times.h>
#include <syscall.h>
#include <time.h>
#include <utime.h>

extern "C" {

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
    if (!tv) {
        errno = EFAULT;
        return -1;
    }

    struct timespec ts = {};
    if (clock_gettime(CLOCK_REALTIME_COARSE, &ts) < 0)
        return -1;

    TIMESPEC_TO_TIMEVAL(tv, &ts);
    return 0;
}

/**
 * @param tv 
 * @param __restrict__ 
 * @return int 
 */
int settimeofday(struct timeval* __restrict__ tv, void* __restrict__)
{
    if (!tv) {
        errno = EFAULT;
        return -1;
    }

    timespec ts;
    TIMEVAL_TO_TIMESPEC(tv, &ts);
    return clock_settime(CLOCK_REALTIME, &ts);
}

/**
 * @param pathname 
 * @param times 
 * @return int 
 */
int utimes(char const* pathname, const struct timeval times[2])
{
    if (!times) {
        return utime(pathname, nullptr);
    }

    utimbuf buf = { times[0].tv_sec, times[1].tv_sec };
    return utime(pathname, &buf);
}

/**
 * @param t 
 * @return char* 
 */
char* ctime(time_t const* t)
{
    return asctime(localtime(t));
}

/**
 * @param t 
 * @param buf 
 * @return char* 
 */
char* ctime_r(time_t const* t, char* buf)
{
    struct tm tm_buf;
    return asctime_r(localtime_r(t, &tm_buf), buf);
}

static int const __seconds_per_day = 60 * 60 * 24;

/**
 * @param tm 
 * @param t 
 * @return struct tm* 
 */
static struct tm* time_to_tm(struct tm* tm, time_t t)
{
    constexpr time_t smallest_possible_time = -67768040609740800;
    constexpr time_t biggest_possible_time = 67768036191676799;
    if (t < smallest_possible_time || t > biggest_possible_time) {
        errno = EOVERFLOW;
        return nullptr;
    }

    int year = 1970;
    for (; t >= days_in_year(year) * __seconds_per_day; ++year)
        t -= days_in_year(year) * __seconds_per_day;
    for (; t < 0; --year)
        t += days_in_year(year - 1) * __seconds_per_day;
    tm->tm_year = year - 1900;

    VERIFY(t >= 0);
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

    return tm;
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
    if (!time_to_tm(tm, timestamp))
        return -1;
    return timestamp;
}

/**
 * @param tm 
 * @return time_t 
 */
time_t mktime(struct tm* tm)
{
    tzset();
    return tm_to_time(tm, daylight ? altzone : timezone);
}

/**
 * @param t 
 * @return struct tm* 
 */
struct tm* localtime(time_t const* t)
{
    tzset();

    static struct tm tm_buf;
    return localtime_r(t, &tm_buf);
}

/**
 * @param t 
 * @param tm 
 * @return struct tm* 
 */
struct tm* localtime_r(time_t const* t, struct tm* tm)
{
    if (!t)
        return nullptr;

    return time_to_tm(tm, *t - (daylight ? altzone : timezone));
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
struct tm* gmtime(time_t const* t)
{
    static struct tm tm_buf;
    return gmtime_r(t, &tm_buf);
}

/**
 * @param t 
 * @param tm 
 * @return struct tm* 
 */
struct tm* gmtime_r(time_t const* t, struct tm* tm)
{
    if (!t)
        return nullptr;
    return time_to_tm(tm, *t);
}

/**
 * @param tm 
 * @return char* 
 */
char* asctime(const struct tm* tm)
{
    static char buffer[69];
    return asctime_r(tm, buffer);
}

/**
 * @param tm 
 * @param buffer 
 * @return char* 
 */
char* asctime_r(const struct tm* tm, char* buffer)
{
    constexpr size_t assumed_len = 26;
    size_t filled_size = strftime(buffer, assumed_len, "%a %b %e %T %Y\n", tm);

    VERIFY(filled_size != 0);

    return buffer;
}

/**
 * @param destination 
 * @param max_size 
 * @param format 
 * @param tm 
 * @return size_t 
 */
size_t strftime(char* destination, size_t max_size, char const* format, const struct tm* tm)
{
    tzset();

    StringBuilder builder { max_size };

    int const format_len = strlen(format);
    for (int i = 0; i < format_len; ++i) {
        if (format[i] != '%') {
            builder.append(format[i]);
        } else {
            if (++i >= format_len)
                return 0;

            switch (format[i]) {
            case 'a':
                builder.append(short_day_names[tm->tm_wday]);
                break;
            case 'A':
                builder.append(long_day_names[tm->tm_wday]);
                break;
            case 'b':
                builder.append(short_month_names[tm->tm_mon]);
                break;
            case 'B':
                builder.append(long_month_names[tm->tm_mon]);
                break;
            case 'C':
                builder.appendff("{:02}", (tm->tm_year + 1900) / 100);
                break;
            case 'd':
                builder.appendff("{:02}", tm->tm_mday);
                break;
            case 'D':
                builder.appendff("{:02}/{:02}/{:02}", tm->tm_mon + 1, tm->tm_mday, (tm->tm_year + 1900) % 100);
                break;
            case 'e':
                builder.appendff("{:2}", tm->tm_mday);
                break;
            case 'h':
                builder.append(short_month_names[tm->tm_mon]);
                break;
            case 'H':
                builder.appendff("{:02}", tm->tm_hour);
                break;
            case 'I': {
                int display_hour = tm->tm_hour % 12;
                if (display_hour == 0)
                    display_hour = 12;
                builder.appendff("{:02}", display_hour);
                break;
            }
            case 'j':
                builder.appendff("{:03}", tm->tm_yday + 1);
                break;
            case 'm':
                builder.appendff("{:02}", tm->tm_mon + 1);
                break;
            case 'M':
                builder.appendff("{:02}", tm->tm_min);
                break;
            case 'n':
                builder.append('\n');
                break;
            case 'p':
                builder.append(tm->tm_hour < 12 ? "AM" : "PM");
                break;
            case 'r': {
                int display_hour = tm->tm_hour % 12;
                if (display_hour == 0)
                    display_hour = 12;
                builder.appendff("{:02}:{:02}:{:02} {}", display_hour, tm->tm_min, tm->tm_sec, tm->tm_hour < 12 ? "AM" : "PM");
                break;
            }
            case 'R':
                builder.appendff("{:02}:{:02}", tm->tm_hour, tm->tm_min);
                break;
            case 'S':
                builder.appendff("{:02}", tm->tm_sec);
                break;
            case 't':
                builder.append('\t');
                break;
            case 'T':
                builder.appendff("{:02}:{:02}:{:02}", tm->tm_hour, tm->tm_min, tm->tm_sec);
                break;
            case 'u':
                builder.appendff("{}", tm->tm_wday ? tm->tm_wday : 7);
                break;
            case 'U': {
                int const wday_of_year_beginning = (tm->tm_wday + 6 * tm->tm_yday) % 7;
                int const week_number = (tm->tm_yday + wday_of_year_beginning) / 7;
                builder.appendff("{:02}", week_number);
                break;
            }
            case 'V': {
                int const wday_of_year_beginning = (tm->tm_wday + 6 + 6 * tm->tm_yday) % 7;
                int week_number = (tm->tm_yday + wday_of_year_beginning) / 7 + 1;
                if (wday_of_year_beginning > 3) {
                    if (tm->tm_yday >= 7 - wday_of_year_beginning)
                        --week_number;
                    else {
                        int const days_of_last_year = days_in_year(tm->tm_year + 1900 - 1);
                        int const wday_of_last_year_beginning = (wday_of_year_beginning + 6 * days_of_last_year) % 7;
                        week_number = (days_of_last_year + wday_of_last_year_beginning) / 7 + 1;
                        if (wday_of_last_year_beginning > 3)
                            --week_number;
                    }
                }
                builder.appendff("{:02}", week_number);
                break;
            }
            case 'w':
                builder.appendff("{}", tm->tm_wday);
                break;
            case 'W': {
                int const wday_of_year_beginning = (tm->tm_wday + 6 + 6 * tm->tm_yday) % 7;
                int const week_number = (tm->tm_yday + wday_of_year_beginning) / 7;
                builder.appendff("{:02}", week_number);
                break;
            }
            case 'y':
                builder.appendff("{:02}", (tm->tm_year + 1900) % 100);
                break;
            case 'Y':
                builder.appendff("{}", tm->tm_year + 1900);
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

static char __tzname_standard[TZNAME_MAX];
static char __tzname_daylight[TZNAME_MAX];
constexpr char const* __utc = "UTC";

long timezone = 0;
long altzone = 0;
char* tzname[2] = { const_cast<char*>(__utc), const_cast<char*>(__utc) };
int daylight = 0;

void tzset()
{
    StringView time_zone;

    if (char* tz = getenv("TZ"); tz != nullptr)
        time_zone = tz;
    else
        time_zone = TimeZone::system_time_zone();

    auto set_default_values = []() {
        timezone = 0;
        altzone = 0;
        daylight = 0;
        tzname[0] = const_cast<char*>(__utc);
        tzname[1] = const_cast<char*>(__utc);
    };

    if (auto offsets = TimeZone::get_named_time_zone_offsets(time_zone, Mods::Time::now_realtime()); offsets.has_value()) {
        if (!offsets->at(0).name.copy_characters_to_buffer(__tzname_standard, TZNAME_MAX))
            return set_default_values();
        if (!offsets->at(1).name.copy_characters_to_buffer(__tzname_daylight, TZNAME_MAX))
            return set_default_values();

        timezone = -offsets->at(0).seconds;
        altzone = -offsets->at(1).seconds;
        daylight = timezone != altzone;
        tzname[0] = __tzname_standard;
        tzname[1] = __tzname_daylight;
    } else {
        set_default_values();
    }
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

static Kernel::TimePage* get_kernel_time_page()
{
    static Kernel::TimePage* s_kernel_time_page;

    if (!s_kernel_time_page) {
        auto rc = syscall(SC_map_time_page);
        if ((int)rc < 0 && (int)rc > -EMAXERRNO) {
            errno = -(int)rc;
            return nullptr;
        }
        s_kernel_time_page = (Kernel::TimePage*)rc;
    }
    return s_kernel_time_page;
}

/**
 * @param clock_id 
 * @param ts 
 * @return int 
 */
int clock_gettime(clockid_t clock_id, struct timespec* ts)
{
    if (Kernel::time_page_supports(clock_id)) {
        if (!ts) {
            errno = EFAULT;
            return -1;
        }

        if (auto* kernel_time_page = get_kernel_time_page()) {
            u32 update_iteration;
            do {
                update_iteration = Mods::atomic_load(&kernel_time_page->update1, Mods::memory_order_acquire);
                *ts = kernel_time_page->clocks[clock_id];
            } while (update_iteration != Mods::atomic_load(&kernel_time_page->update2, Mods::memory_order_acquire));
            return 0;
        }
    }

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
    dbgln("FIXME: Implement clock_getres()");
    auto rc = -ENOSYS;
    __RETURN_WITH_ERRNO(rc, rc, -1);
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
