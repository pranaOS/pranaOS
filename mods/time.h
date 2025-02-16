/**
 * @file time.h
 * @author Krisna Pranav
 * @brief Time
 * @version 6.0
 * @date 2023-07-04
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/array.h>
#include <mods/assertions.h>
#include <mods/platform.h>
#include <mods/types.h>

struct timeval;
struct timespec;

namespace Mods
{
    /**
     * @tparam T 
     */
    template <typename T>
    concept TimeSpecType = requires(T t) {
        t.tv_sec;
        t.tv_nsec;
    };

    /**
     * @param year 
     * @return true 
     * @return false 
     */
    constexpr bool is_leap_year(int year)
    {
        return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
    }

    /**
     * @param year 
     * @param month 
     * @param day 
     * @return unsigned 
     */
    unsigned day_of_week(int year, unsigned month, int day);

    /**
     * @param year 
     * @param month 
     * @param day 
     * @return constexpr int 
     */
    constexpr int day_of_year(int year, unsigned month, int day)
    {
        VERIFY(month >= 1 && month <= 12);

        constexpr Array seek_table = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
        int day_of_year = seek_table[month - 1] + day - 1;

        if(is_leap_year(year) && month >= 3)
            day_of_year++;

        return day_of_year;
    }

    /**
     * @param year 
     * @param month 
     * @return int 
     */
    int days_in_month(int year, unsigned month);

    /**
     * @param year 
     * @return constexpr int 
     */
    constexpr int days_in_year(int year)
    {
        return 365 + (is_leap_year(year) ? 1 : 0);
    }

    /**
     * @param year 
     * @return constexpr int 
     */
    constexpr int years_to_days_since_epoch(int year)
    {
        int days = 0;
        for(int current_year = 1970; current_year < year; ++current_year)
            days += days_in_year(current_year);
        for(int current_year = year; current_year < 1970; ++current_year)
            days -= days_in_year(current_year);
        return days;
    }

    /**
     * @param year 
     * @param month 
     * @param day 
     * @return constexpr int 
     */
    constexpr int days_since_epoch(int year, int month, int day)
    {
        return years_to_days_since_epoch(year) + day_of_year(year, month, day);
    }

    /**
     * @param seconds 
     * @return constexpr i64 
     */
    constexpr i64 seconds_since_epoch_to_year(i64 seconds)
    {
        constexpr double seconds_per_year = 60.0 * 60.0 * 24.0 * 365.2425;

        auto round_down = [](double value) -> i64
        {
            auto as_i64 = static_cast<i64>(value);

            if((value == as_i64) || (as_i64 >= 0))
                return as_i64;
            return as_i64 - 1;
        };

        auto years_since_epoch = static_cast<double>(seconds) / seconds_per_year;
        return 1970 + round_down(years_since_epoch);
    }

    class Time
    {
    public:
        /**
         * @brief Construct a new Time object
         * 
         */
        Time() = default;

        /**
         * @brief Construct a new Time object
         * 
         */
        Time(Time const&) = default;

        /**
         * @return Time& 
         */
        Time& operator=(Time const&) = default;

        /**
         * @brief Construct a new Time object
         * 
         * @param other 
         */
        Time(Time&& other)
            : m_seconds(exchange(other.m_seconds, 0)), m_nanoseconds(exchange(other.m_nanoseconds, 0))
        {
        }

        /**
         * @param other 
         * @return Time& 
         */
        Time& operator=(Time&& other)
        {
            if(this != &other)
            {
                m_seconds = exchange(other.m_seconds, 0);
                m_nanoseconds = exchange(other.m_nanoseconds, 0);
            }
            return *this;
        }

    private:
        /**
         * @param numerator 
         * @param denominator 
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE static constexpr i64 sane_mod(i64& numerator, i64 denominator)
        {
            VERIFY(2 <= denominator && denominator <= 1'000'000'000);

            i64 dividend = numerator / denominator;
            numerator %= denominator;
            if(numerator < 0)
            {
                numerator += denominator;
                dividend -= 1;
            }
            return dividend;
        }

        /**
         * @param numerator 
         * @param denominator 
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE static constexpr i32 sane_mod(i32& numerator, i32 denominator)
        {
            i64 numerator_64 = numerator;
            i64 dividend = sane_mod(numerator_64, denominator);
            
            numerator = static_cast<i32>(numerator_64);

            return static_cast<i32>(dividend);
        }

    public:
        /**
         * @param year 
         * @param month 
         * @param day 
         * @param hour 
         * @param minute 
         * @param second 
         * @param millisecond 
         * @return constexpr Time 
         */
        [[nodiscard]] constexpr static Time from_timestamp(i32 year, u8 month, u8 day, u8 hour, u8 minute, u8 second, u16 millisecond)
        {
            constexpr auto milliseconds_per_day = 86'400'000;
            constexpr auto milliseconds_per_hour = 3'600'000;
            constexpr auto milliseconds_per_minute = 60'000;
            constexpr auto milliseconds_per_second = 1'000;

            i64 milliseconds_since_epoch = days_since_epoch(year, month, day);
            milliseconds_since_epoch *= milliseconds_per_day;

            milliseconds_since_epoch += hour * milliseconds_per_hour;
            milliseconds_since_epoch += minute * milliseconds_per_minute;
            milliseconds_since_epoch += second * milliseconds_per_second;
            milliseconds_since_epoch += millisecond;

            return from_milliseconds(milliseconds_since_epoch);
        }

        /**
         * @param seconds 
         * @return constexpr Time 
         */
        [[nodiscard]] constexpr static Time from_seconds(i64 seconds)
        {
            return Time(seconds, 0);
        }

        /**
         * @param nanoseconds 
         * @return constexpr Time 
         */
        [[nodiscard]] constexpr static Time from_nanoseconds(i64 nanoseconds)
        {
            i64 seconds = sane_mod(nanoseconds, 1'000'000'000);
            return Time(seconds, nanoseconds);
        }

        /**
         * @param microseconds 
         * @return constexpr Time 
         */
        [[nodiscard]] constexpr static Time from_microseconds(i64 microseconds)
        {
            i64 seconds = sane_mod(microseconds, 1'000'000);
            return Time(seconds, microseconds * 1'000);
        }

        /**
         * @param milliseconds 
         * @return constexpr Time 
         */
        [[nodiscard]] constexpr static Time from_milliseconds(i64 milliseconds)
        {
            i64 seconds = sane_mod(milliseconds, 1'000);
            return Time(seconds, milliseconds * 1'000'000);
        }

        /**
         * @return Time 
         */
        [[nodiscard]] static Time from_ticks(clock_t, time_t);

        /**
         * @return Time 
         */
        [[nodiscard]] static Time from_timespec(const struct timespec&);

        /**
         * @return Time 
         */
        [[nodiscard]] static Time from_timeval(const struct timeval&);
        
        /**
         * @return constexpr Time 
         */
        [[nodiscard]] constexpr static Time min()
        {
            return Time(-__INT64_MAX__ - 1LL, 0);
        };

        /**
         * @return constexpr Time 
         */
        [[nodiscard]] constexpr static Time zero()
        {
            return Time(0, 0);
        };

        /**
         * @return constexpr Time 
         */
        [[nodiscard]] constexpr static Time max()
        {
            return Time(__INT64_MAX__, 999'999'999);
        };

    #ifndef KERNEL
        [[nodiscard]] static Time now_realtime();
        [[nodiscard]] static Time now_realtime_coarse();
        [[nodiscard]] static Time now_monotonic();
        [[nodiscard]] static Time now_monotonic_coarse();
    #endif

        /**
         * @return i64 
         */
        [[nodiscard]] i64 to_truncated_seconds() const;
        [[nodiscard]] i64 to_truncated_milliseconds() const;
        [[nodiscard]] i64 to_truncated_microseconds() const;
        
        [[nodiscard]] i64 to_seconds() const;
        [[nodiscard]] i64 to_milliseconds() const;
        [[nodiscard]] i64 to_microseconds() const;
        [[nodiscard]] i64 to_nanoseconds() const;
        [[nodiscard]] timespec to_timespec() const;
        
        [[nodiscard]] timeval to_timeval() const;

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool is_zero() const
        {
            return (m_seconds == 0) && (m_nanoseconds == 0);
        }

        [[nodiscard]] bool is_negative() const
        {
            return m_seconds < 0;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(Time const& other) const
        {
            return this->m_seconds == other.m_seconds && this->m_nanoseconds == other.m_nanoseconds;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(Time const& other) const
        {
            return !(*this == other);
        }

        /**
         * @param other 
         * @return Time 
         */
        Time operator+(Time const& other) const;

        /**
         * @param other 
         * @return Time& 
         */
        Time& operator+=(Time const& other);

        /**
         * @param other 
         * @return Time 
         */
        Time operator-(Time const& other) const;

        /**
         * @param other 
         * @return Time& 
         */
        Time& operator-=(Time const& other);

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator<(Time const& other) const;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator<=(Time const& other) const;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator>(Time const& other) const;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator>=(Time const& other) const;

    private:
        constexpr explicit Time(i64 seconds, u32 nanoseconds)
            : m_seconds(seconds), m_nanoseconds(nanoseconds)
        {
        }

        /**
         * @param seconds 
         * @param extra_seconds 
         * @param nanoseconds 
         * @return Time 
         */
        [[nodiscard]] static Time from_half_sanitized(i64 seconds, i32 extra_seconds, u32 nanoseconds);

        i64 m_seconds{0};
        u32 m_nanoseconds{0}; 
    }; // class Time

    /**
     * @tparam TimevalType 
     * @param a 
     * @param b 
     * @param result 
     */
    template <typename TimevalType>
    inline void timeval_sub(TimevalType const& a, TimevalType const& b, TimevalType& result)
    {
        result.tv_sec = a.tv_sec - b.tv_sec;
        result.tv_usec = a.tv_usec - b.tv_usec;
        if(result.tv_usec < 0)
        {
            --result.tv_sec;
            result.tv_usec += 1'000'000;
        }
    }

    /**
     * @tparam TimevalType 
     * @param a 
     * @param b 
     * @param result 
     */
    template <typename TimevalType>
    inline void timeval_add(TimevalType const& a, TimevalType const& b, TimevalType& result)
    {
        result.tv_sec = a.tv_sec + b.tv_sec;
        result.tv_usec = a.tv_usec + b.tv_usec;
        if(result.tv_usec >= 1'000'000)
        {
            ++result.tv_sec;
            result.tv_usec -= 1'000'000;
        }
    }

    /**
     * @tparam TimespecType 
     * @param a 
     * @param b 
     * @param result 
     */
    template <typename TimespecType>
    inline void timespec_sub(TimespecType const& a, TimespecType const& b, TimespecType& result)
    {
        result.tv_sec = a.tv_sec - b.tv_sec;
        result.tv_nsec = a.tv_nsec - b.tv_nsec;
        if(result.tv_nsec < 0)
        {
            --result.tv_sec;
            result.tv_nsec += 1'000'000'000;
        }
    }

    /**
     * @tparam TimespecType 
     * @param a 
     * @param b 
     * @param result 
     */
    template <typename TimespecType>
    inline void timespec_add(TimespecType const& a, TimespecType const& b, TimespecType& result)
    {
        result.tv_sec = a.tv_sec + b.tv_sec;
        result.tv_nsec = a.tv_nsec + b.tv_nsec;
        if(result.tv_nsec >= 1000'000'000)
        {
            ++result.tv_sec;
            result.tv_nsec -= 1000'000'000;
        }
    }

    /**
     * @tparam TimespecType 
     * @tparam TimevalType 
     * @param a 
     * @param b 
     * @param result 
     */
    template <typename TimespecType, typename TimevalType>
    inline void timespec_add_timeval(TimespecType const& a, TimevalType const& b, TimespecType& result)
    {
        result.tv_sec = a.tv_sec + b.tv_sec;
        result.tv_nsec = a.tv_nsec + b.tv_usec * 1000;
        if(result.tv_nsec >= 1000'000'000)
        {
            ++result.tv_sec;
            result.tv_nsec -= 1000'000'000;
        }
    }

    /**
     * @tparam TimevalType 
     * @tparam TimespecType 
     * @param tv 
     * @param ts 
     */
    template <typename TimevalType, typename TimespecType>
    inline void timeval_to_timespec(TimevalType const& tv, TimespecType& ts)
    {
        ts.tv_sec = tv.tv_sec;
        ts.tv_nsec = tv.tv_usec * 1000;
    }

    /**
     * @tparam TimespecType 
     * @tparam TimevalType 
     * @param ts 
     * @param tv 
     */
    template <typename TimespecType, typename TimevalType>
    inline void timespec_to_timeval(TimespecType const& ts, TimevalType& tv)
    {
        tv.tv_sec = ts.tv_sec;
        tv.tv_usec = ts.tv_nsec / 1000;
    }

    /**
     * @tparam T 
     * @param a 
     * @param b 
     * @return true 
     * @return false 
     */
    template <TimeSpecType T>
    inline bool operator>=(const T& a, const T& b)
    {
        return a.tv_sec > b.tv_sec || (a.tv_sec == b.tv_sec && a.tv_nsec >= b.tv_nsec);
    }

    /**
     * @tparam T 
     * @param a 
     * @param b 
     * @return true 
     * @return false 
     */
    template <TimeSpecType T>
    inline bool operator>(const T& a, const T& b)
    {
        return a.tv_sec > b.tv_sec || (a.tv_sec == b.tv_sec && a.tv_nsec > b.tv_nsec);
    }

    /**
     * @tparam T 
     * @param a 
     * @param b 
     * @return true 
     * @return false 
     */
    template <TimeSpecType T>
    inline bool operator<(const T& a, const T& b)
    {
        return a.tv_sec < b.tv_sec || (a.tv_sec == b.tv_sec && a.tv_nsec < b.tv_nsec);
    }

    /**
     * @tparam T 
     * @param a 
     * @param b 
     * @return true 
     * @return false 
     */
    template <TimeSpecType T>
    inline bool operator<=(const T& a, const T& b)
    {
        return a.tv_sec < b.tv_sec || (a.tv_sec == b.tv_sec && a.tv_nsec <= b.tv_nsec);
    }

    /**
     * @tparam T 
     * @param a 
     * @param b 
     * @return true 
     * @return false 
     */
    template <TimeSpecType T>
    inline bool operator==(const T& a, const T& b)
    {
        return a.tv_sec == b.tv_sec && a.tv_nsec == b.tv_nsec;
    }
    
    /**
     * @tparam T 
     * @param a 
     * @param b 
     * @return true 
     * @return false 
     */
    template <TimeSpecType T>
    inline bool operator!=(const T& a, const T& b)
    {
        return a.tv_sec != b.tv_sec || a.tv_nsec != b.tv_nsec;
    }
} // namespace Mods

using Mods::day_of_week;
using Mods::day_of_year;
using Mods::days_in_month;
using Mods::days_in_year;
using Mods::days_since_epoch;
using Mods::is_leap_year;
using Mods::seconds_since_epoch_to_year;
using Mods::Time;
using Mods::timespec_add;
using Mods::timespec_add_timeval;
using Mods::timespec_sub;
using Mods::timespec_to_timeval;
using Mods::timeval_add;
using Mods::timeval_sub;
using Mods::timeval_to_timespec;
using Mods::years_to_days_since_epoch;
using Mods::operator<=;
using Mods::operator<;
using Mods::operator>;
using Mods::operator>=;
using Mods::operator==;
using Mods::operator!=;
