/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Assertions.h>
#include <base/Platform.h>
#include <base/Types.h>

struct timeval;
struct timespec;

template<typename T>
concept TimeSpecType = requires(T t)
{
    t.tv_sec;
    t.tv_nsec;
};

namespace Base {

unsigned day_of_weak(int year, unsigned month, int day);

int day_of_year(int year, unsigned month, int day);

int days_in_month(int year, unsigned month);

inline bool is_leap_year(int year)
{
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

inline unsigned days_in_year(int year)
{
    return 365 + is_leap_year(year);
}

inline int years_to_days_since_epoch(int year)
{
    int days = 0;
    for (int current_year = 1970; current_year < year; ++current_year)
        days += days_in_year(current_year);
    for (int current_year = year; current_year < 1970; ++current_year)
        days -= days_in_year(current_year);
    return days;
}

class Time {
public:
    Time() = default;
    Time(const Time&) = default;
    Time& operator=(const Time&) = default;

    Time(Time&& other)
        : m_seconds(exchange(other.m_seconds, 0))
        , m_nanoseconds(exchange(other.m_nanoseconds, 0))
    {
    }
    Time& operator=(Time&& other)
    {
        if (this != &other) {
            m_seconds = exchange(other.m_seconds, 0);
            m_nanoseconds = exchange(other.m_nanoseconds, 0);
        }
        return *this;
    }

private:
    ALWAYS_INLINE static constexpr i64 sane_mod(i64& numerator, i64 denominator)
    {
        VERIFY(2 <= denominator && denominator <= 1'000'000'000);
        
        i64 dividend = numerator / denominator;
        numerator %= denominator;
        if (numerator < 0) {
            
            numerator += denominator;
            dividend -= 1;
        }
        return dividend;
    }
    ALWAYS_INLINE static constexpr i32 sane_mod(i32& numerator, i32 denominator)
    {
        i64 numerator_64 = numerator;
        i64 dividend = sane_mod(numerator_64, denominator);
        numerator = numerator_64;
        return dividend;
    }

public:
    constexpr static Time from_seconds(i64 seconds) { return Time(seconds, 0); }
    constexpr static Time from_nanoseconds(i64 nanoseconds)
    {
        i64 seconds = sane_mod(nanoseconds, 1'000'000'000);
        return Time(seconds, nanoseconds);
    }
    constexpr static Time from_microseconds(i64 microseconds)
    {
        i64 seconds = sane_mod(microseconds, 1'000'000);
        return Time(seconds, microseconds * 1'000);
    }
    constexpr static Time from_milliseconds(i64 milliseconds)
    {
        i64 seconds = sane_mod(milliseconds, 1'000);
        return Time(seconds, milliseconds * 1'000'000);
    }
    static Time from_timespec(const struct timespec&);
    static Time from_timeval(const struct timeval&);
    static Time min() { return Time(-0x8000'0000'0000'0000LL, 0); };
    static Time zero() { return Time(0, 0); };
    static Time max() { return Time(0x7fff'ffff'ffff'ffffLL, 999'999'999); };

    i64 to_truncated_seconds() const;
    i64 to_truncated_milliseconds() const;
    i64 to_truncated_microseconds() const;
    i64 to_seconds() const;
    i64 to_milliseconds() const;
    i64 to_microseconds() const;
    i64 to_nanoseconds() const;
    timespec to_timespec() const;

    timeval to_timeval() const;

    bool is_zero() const { return !m_seconds && !m_nanoseconds; }

    bool operator==(const Time& other) const { return this->m_seconds == other.m_seconds && this->m_nanoseconds == other.m_nanoseconds; }
    bool operator!=(const Time& other) const { return !(*this == other); }
    Time operator+(const Time& other) const;
    Time& operator+=(const Time& other);
    Time operator-(const Time& other) const;
    Time& operator-=(const Time& other);
    bool operator<(const Time& other) const;
    bool operator<=(const Time& other) const;
    bool operator>(const Time& other) const;
    bool operator>=(const Time& other) const;

private:
    constexpr explicit Time(i64 seconds, u32 nanoseconds)
        : m_seconds(seconds)
        , m_nanoseconds(nanoseconds)
    {
    }

    static Time from_half_sanitized(i64 seconds, i32 extra_seconds, u32 nanoseconds);

    i64 m_seconds { 0 };
    u32 m_nanoseconds { 0 }; 
};
}