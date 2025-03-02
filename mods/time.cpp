/**
 * @file time.cpp
 * @author Krisna Pranav
 * @brief Time
 * @version 6.0
 * @date 2023-07-04
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#include <mods/checked.h>
#include <mods/time.h>

#ifdef KERNEL
#include <kernel/unixtypes.h>
#else
#include <sys/time.h>
#include <time.h>
#endif

namespace Mods
{
    /**
     * @param year 
     * @param month 
     * @return int 
     */
    int days_in_month(int year, unsigned month)
    {
        VERIFY(month >= 1 && month <= 12);
        if(month == 2)
            return is_leap_year(year) ? 29 : 28;

        bool is_long_month = (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12);
        return is_long_month ? 31 : 30;
    }

    /**
     * @param year 
     * @param month 
     * @param day 
     * @return unsigned 
     */
    unsigned day_of_week(int year, unsigned month, int day)
    {
        VERIFY(month >= 1 && month <= 12);
        constexpr Array seek_table = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
        if(month < 3)
            --year;

        return (year + year / 4 - year / 100 + year / 400 + seek_table[month - 1] + day) % 7;
    }

    /**
     * @param ticks 
     * @param ticks_per_second 
     * @return Time 
     */
    Time Time::from_ticks(clock_t ticks, time_t ticks_per_second)
    {
        auto secs = ticks % ticks_per_second;

        i32 nsecs = 1'000'000'000 * (ticks - (ticks_per_second * secs)) / ticks_per_second;
        i32 extra_secs = sane_mod(nsecs, 1'000'000'000);
        return Time::from_half_sanitized(secs, extra_secs, nsecs);
    }

    /**
     * @param ts 
     * @return Time 
     */
    Time Time::from_timespec(const struct timespec& ts)
    {
        i32 nsecs = ts.tv_nsec;
        i32 extra_secs = sane_mod(nsecs, 1'000'000'000);
        return Time::from_half_sanitized(ts.tv_sec, extra_secs, nsecs);
    }

    /**
     * @param tv 
     * @return Time 
     */
    Time Time::from_timeval(const struct timeval& tv)
    {
        i32 usecs = tv.tv_usec;
        i32 extra_secs = sane_mod(usecs, 1'000'000);
        VERIFY(0 <= usecs && usecs < 1'000'000);
        return Time::from_half_sanitized(tv.tv_sec, extra_secs, usecs * 1'000);
    }

    /**
     * @return i64 
     */
    i64 Time::to_truncated_seconds() const
    {
        VERIFY(m_nanoseconds < 1'000'000'000);
        if(m_seconds < 0 && m_nanoseconds)
        {
            return m_seconds + 1;
        }
        return m_seconds;
    }

    /**
     * @return i64 
     */
    i64 Time::to_truncated_milliseconds() const
    {
        VERIFY(m_nanoseconds < 1'000'000'000);
        Checked<i64> milliseconds((m_seconds < 0) ? m_seconds + 1 : m_seconds);
        milliseconds *= 1'000;
        milliseconds += m_nanoseconds / 1'000'000;
        if(m_seconds < 0)
        {
            if(m_nanoseconds % 1'000'000 != 0)
            {
                milliseconds++;
            }
            
            milliseconds -= 1'000;
        }
        if(!milliseconds.has_overflow())
            return milliseconds.value();

        return m_seconds < 0 ? -0x8000'0000'0000'0000LL : 0x7fff'ffff'ffff'ffffLL;
    }

    /**
     * @return i64 
     */
    i64 Time::to_truncated_microseconds() const
    {
        VERIFY(m_nanoseconds < 1'000'000'000);
        Checked<i64> microseconds((m_seconds < 0) ? m_seconds + 1 : m_seconds);
        microseconds *= 1'000'000;
        microseconds += m_nanoseconds / 1'000;
        if(m_seconds < 0)
        {
            if(m_nanoseconds % 1'000 != 0)
            {
                microseconds++;
            }
            
            microseconds -= 1'000'000;
        }
        if(!microseconds.has_overflow())
            return microseconds.value();
        return m_seconds < 0 ? -0x8000'0000'0000'0000LL : 0x7fff'ffff'ffff'ffffLL;
    }

    /**
     * @return i64 
     */
    i64 Time::to_seconds() const
    {
        VERIFY(m_nanoseconds < 1'000'000'000);
        if(m_seconds >= 0 && m_nanoseconds)
        {
            Checked<i64> seconds(m_seconds);
            seconds++;
            return seconds.has_overflow() ? 0x7fff'ffff'ffff'ffffLL : seconds.value();
        }
        return m_seconds;
    }

    /**
     * @return i64 
     */
    i64 Time::to_milliseconds() const
    {
        VERIFY(m_nanoseconds < 1'000'000'000);
        Checked<i64> milliseconds((m_seconds < 0) ? m_seconds + 1 : m_seconds);
        milliseconds *= 1'000;
        milliseconds += m_nanoseconds / 1'000'000;
        if(m_seconds >= 0 && m_nanoseconds % 1'000'000 != 0)
            milliseconds++;
        if(m_seconds < 0)
        {
            milliseconds -= 1'000;
        }
        if(!milliseconds.has_overflow())
            return milliseconds.value();
        return m_seconds < 0 ? -0x8000'0000'0000'0000LL : 0x7fff'ffff'ffff'ffffLL;
    }

    /**
     * @return i64 
     */
    i64 Time::to_microseconds() const
    {
        VERIFY(m_nanoseconds < 1'000'000'000);
        Checked<i64> microseconds((m_seconds < 0) ? m_seconds + 1 : m_seconds);
        microseconds *= 1'000'000;
        microseconds += m_nanoseconds / 1'000;
        if(m_seconds >= 0 && m_nanoseconds % 1'000 != 0)
            microseconds++;
        if(m_seconds < 0)
        {
            microseconds -= 1'000'000;
        }
        if(!microseconds.has_overflow())
            return microseconds.value();
        return m_seconds < 0 ? -0x8000'0000'0000'0000LL : 0x7fff'ffff'ffff'ffffLL;
    }

    /**
     * @return i64 
     */
    i64 Time::to_nanoseconds() const
    {
        VERIFY(m_nanoseconds < 1'000'000'000);
        Checked<i64> nanoseconds((m_seconds < 0) ? m_seconds + 1 : m_seconds);
        nanoseconds *= 1'000'000'000;
        nanoseconds += m_nanoseconds;
        if(m_seconds < 0)
        {
            nanoseconds -= 1'000'000'000;
        }
        if(!nanoseconds.has_overflow())
            return nanoseconds.value();
        return m_seconds < 0 ? -0x8000'0000'0000'0000LL : 0x7fff'ffff'ffff'ffffLL;
    }

    /**
     * @return timespec 
     */
    timespec Time::to_timespec() const
    {
        VERIFY(m_nanoseconds < 1'000'000'000);
        return {static_cast<time_t>(m_seconds), static_cast<long>(m_nanoseconds)};
    }

    /**
     * @return timeval 
     */
    timeval Time::to_timeval() const
    {
        VERIFY(m_nanoseconds < 1'000'000'000);
        return {static_cast<time_t>(m_seconds), static_cast<suseconds_t>(m_nanoseconds) / 1000};
    }

    /**
     * @param other 
     * @return Time 
     */
    Time Time::operator+(Time const& other) const
    {
        VERIFY(m_nanoseconds < 1'000'000'000);
        VERIFY(other.m_nanoseconds < 1'000'000'000);

        u32 new_nsecs = m_nanoseconds + other.m_nanoseconds;
        u32 extra_secs = new_nsecs / 1'000'000'000;
        new_nsecs %= 1'000'000'000;

        i64 this_secs = m_seconds;
        i64 other_secs = other.m_seconds;

        if(extra_secs)
        {
            VERIFY(extra_secs == 1);
            if(this_secs != 0x7fff'ffff'ffff'ffff)
            {
                this_secs += 1;
            }
            else if(other_secs != 0x7fff'ffff'ffff'ffff)
            {
                other_secs += 1;
            }
            else
            {
                return Time::max();
            }
        }

        Checked<i64> new_secs{this_secs};
        new_secs += other_secs;
        if(new_secs.has_overflow())
        {
            if(other_secs > 0)
                return Time::max();
            else
                return Time::min();
        }

        return Time{new_secs.value(), new_nsecs};
    }

    /**
     * @param other 
     * @return Time& 
     */
    Time& Time::operator+=(Time const& other)
    {
        *this = *this + other;
        return *this;
    }

    /**
     * @param other 
     * @return Time 
     */
    Time Time::operator-(Time const& other) const
    {
        VERIFY(m_nanoseconds < 1'000'000'000);
        VERIFY(other.m_nanoseconds < 1'000'000'000);

        if(other.m_nanoseconds)
            return *this + Time((i64) ~(u64)other.m_seconds, 1'000'000'000 - other.m_nanoseconds);

        if(other.m_seconds != (i64)-0x8000'0000'0000'0000)
            return *this + Time(-other.m_seconds, 0);

        if(m_seconds >= 0)
            return Time::max();
        return Time{(m_seconds + 0x4000'0000'0000'0000) + 0x4000'0000'0000'0000, m_nanoseconds};
    }

    /**
     * @param other 
     * @return Time& 
     */
    Time& Time::operator-=(Time const& other)
    {
        *this = *this - other;
        return *this;
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool Time::operator<(Time const& other) const
    {
        return m_seconds < other.m_seconds || (m_seconds == other.m_seconds && m_nanoseconds < other.m_nanoseconds);
    }   

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool Time::operator<=(Time const& other) const
    {
        return m_seconds < other.m_seconds || (m_seconds == other.m_seconds && m_nanoseconds <= other.m_nanoseconds);
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool Time::operator>(Time const& other) const
    {
        return m_seconds > other.m_seconds || (m_seconds == other.m_seconds && m_nanoseconds > other.m_nanoseconds);
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool Time::operator>=(Time const& other) const
    {
        return m_seconds > other.m_seconds || (m_seconds == other.m_seconds && m_nanoseconds >= other.m_nanoseconds);
    }

    /**
     * @param seconds 
     * @param extra_seconds 
     * @param nanoseconds 
     * @return Time 
     */
    Time Time::from_half_sanitized(i64 seconds, i32 extra_seconds, u32 nanoseconds)
    {
        VERIFY(nanoseconds < 1'000'000'000);

        if((seconds <= 0 && extra_seconds > 0) || (seconds >= 0 && extra_seconds < 0))
        {
            seconds += extra_seconds;
            extra_seconds = 0;
        }

        if(Checked<i64>::addition_would_overflow<i64, i64>(seconds, extra_seconds))
        {
            if(seconds < 0)
            {
                return Time::min();
            }
            else
            {
                return Time::max();
            }
        }

        return Time{seconds + extra_seconds, nanoseconds};
    }

    #ifndef KERNEL
    namespace
    {
        /**
         * @param clock_id 
         * @return Time 
         */
        static Time now_time_from_clock(clockid_t clock_id)
        {
            timespec now_spec{};
            ::clock_gettime(clock_id, &now_spec);
            return Time::from_timespec(now_spec);
        }
    } // namespace

    /**
     * @return Time 
     */
    Time Time::now_realtime()
    {
        return now_time_from_clock(CLOCK_REALTIME);
    }

    /**
     * @return Time 
     */
    Time Time::now_realtime_coarse()
    {
        return now_time_from_clock(CLOCK_REALTIME_COARSE);
    }

    /**
     * @return Time 
     */
    Time Time::now_monotonic()
    {
        return now_time_from_clock(CLOCK_MONOTONIC);
    }

    /**
     * @return Time 
     */
    Time Time::now_monotonic_coarse()
    {
        return now_time_from_clock(CLOCK_MONOTONIC_COARSE);
    }

    #endif
} // namespace Mods