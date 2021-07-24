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

};

}