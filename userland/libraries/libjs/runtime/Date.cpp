/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/StringBuilder.h>
#include <libcore/DateTime.h>
#include <libjs/runtime/Date.h>
#include <libjs/runtime/GlobalObject.h>
#include <time.h>

namespace JS {

Date* Date::create(GlobalObject& global_object, Core::DateTime datetime, i16 milliseconds, bool is_invalid)
{
    return global_object.heap().allocate<Date>(global_object, datetime, milliseconds, is_invalid, *global_object.date_prototype());
}

Date::Date(Core::DateTime datetime, i16 milliseconds, bool is_invalid, Object& prototype)
    : Object(prototype)
    , m_datetime(datetime)
    , m_milliseconds(milliseconds)
    , m_is_invalid(is_invalid)
{
}

Date::~Date()
{
}

tm Date::to_utc_tm() const
{
    time_t timestamp = m_datetime.timestamp();
    struct tm tm;
    gmtime_r(&timestamp, &tm);
    return tm;
}

int Date::utc_date() const
{
    return to_utc_tm().tm_mday;
}

int Date::utc_day() const
{
    return to_utc_tm().tm_wday;
}

int Date::utc_full_year() const
{
    return to_utc_tm().tm_year + 1900;
}

int Date::utc_hours() const
{
    return to_utc_tm().tm_hour;
}

int Date::utc_minutes() const
{
    return to_utc_tm().tm_min;
}

int Date::utc_month() const
{
    return to_utc_tm().tm_mon;
}

int Date::utc_seconds() const
{
    return to_utc_tm().tm_sec;
}

String Date::gmt_date_string() const
{
    
    return datetime().to_string("%a, %e %b %Y %T GMT");
}

String Date::iso_date_string() const
{
    auto tm = to_utc_tm();
    int year = tm.tm_year + 1900;
    int month = tm.tm_mon + 1;

    StringBuilder builder;
    if (year < 0)
        builder.appendff("-{:06}", -year);
    else if (year > 9999)
        builder.appendff("+{:06}", year);
    else
        builder.appendff("{:04}", year);
    builder.append('-');
    builder.appendff("{:02}", month);
    builder.append('-');
    builder.appendff("{:02}", tm.tm_mday);
    builder.append('T');
    builder.appendff("{:02}", tm.tm_hour);
    builder.append(':');
    builder.appendff("{:02}", tm.tm_min);
    builder.append(':');
    builder.appendff("{:02}", tm.tm_sec);
    builder.append('.');
    builder.appendff("{:03}", m_milliseconds);
    builder.append('Z');

    return builder.build();
}

static constexpr double HOURS_PER_DAY = 24;
static constexpr double MINUTES_PER_HOUR = 60;
static constexpr double SECONDS_PER_MINUTE = 60;
static constexpr double MS_PER_SECOND = 1000;
static constexpr double MS_PER_MINUTE = 60000;
static constexpr double MS_PER_HOUR = 3600000;

static constexpr double MS_PER_DAY = 86400000;


u16 day_within_year(double t)
{
    return static_cast<u16>(day(t) - day_from_year(year_from_time(t)));
}

u8 date_from_time(double t)
{
    switch (month_from_time(t)) {

    case 0:
        return day_within_year(t) + 1;
    case 1:
        return day_within_year(t) - 30;
    case 2:
        return day_within_year(t) - 58 - in_leap_year(t);
    case 3:
        return day_within_year(t) - 89 - in_leap_year(t);
    case 4:
        return day_within_year(t) - 119 - in_leap_year(t);
    case 5:
        return day_within_year(t) - 150 - in_leap_year(t);
    case 6:
        return day_within_year(t) - 180 - in_leap_year(t);
    case 7:
        return day_within_year(t) - 211 - in_leap_year(t);
    case 8:
        return day_within_year(t) - 242 - in_leap_year(t);
    case 9:
        return day_within_year(t) - 272 - in_leap_year(t);
    case 10:
        return day_within_year(t) - 303 - in_leap_year(t);
    case 11:
        return day_within_year(t) - 333 - in_leap_year(t);
    default:
        VERIFY_NOT_REACHED();
    }
}

u16 days_in_year(i32 y)
{
    if (y % 4 != 0)
        return 365;
    if (y % 4 == 0 && y % 100 != 0)
        return 366;
    if (y % 100 == 0 && y % 400 != 0)
        return 365;
    if (y % 400 == 0)
        return 366;
    VERIFY_NOT_REACHED();
}

double day_from_year(i32 y)
{
    return 365 * (y - 1970) + floor((y - 1969) / 4.0) - floor((y - 1901) / 100.0) + floor((y - 1601) / 400.0);
}

i32 year_from_time(double t)
{
    return static_cast<i32>(t / (365.0 * MS_PER_DAY) + 1970);
}

bool in_leap_year(double t)
{
    return days_in_year(year_from_time(t)) == 366;
}

u8 month_from_time(double t)
{
    auto in_leap_year = JS::in_leap_year(t);
    auto day_within_year = JS::day_within_year(t);

    if (day_within_year < 31)
        return 0;

    if (31 <= day_within_year && day_within_year < 59 + in_leap_year)
        return 1;
    if (59 + in_leap_year <= day_within_year && day_within_year < 90 + in_leap_year)
        return 2;

    if (90 + in_leap_year <= day_within_year && day_within_year < 120 + in_leap_year)
        return 3;

    if (120 + in_leap_year <= day_within_year && day_within_year < 151 + in_leap_year)
        return 4;

    if (151 + in_leap_year <= day_within_year && day_within_year < 181 + in_leap_year)
        return 5;
    if (181 + in_leap_year <= day_within_year && day_within_year < 212 + in_leap_year)
        return 6;

    if (212 + in_leap_year <= day_within_year && day_within_year < 243 + in_leap_year)
        return 7;
    
    if (243 + in_leap_year <= day_within_year && day_within_year < 273 + in_leap_year)
        return 8;
    
    if (273 + in_leap_year <= day_within_year && day_within_year < 304 + in_leap_year)
        return 9;
    
    if (304 + in_leap_year <= day_within_year && day_within_year < 334 + in_leap_year)
        return 10;

    if (334 + in_leap_year <= day_within_year && day_within_year < 365 + in_leap_year)
        return 11;
    VERIFY_NOT_REACHED();
}

u8 hour_from_time(double t)
{
    return static_cast<u8>(fmod(floor(t / MS_PER_HOUR), HOURS_PER_DAY));
}

u8 min_from_time(double t)
{
    return static_cast<u8>(fmod(floor(t / MS_PER_MINUTE), MINUTES_PER_HOUR));
}

u8 sec_from_time(double t)
{
    return static_cast<u8>(fmod(t / MS_PER_SECOND, SECONDS_PER_MINUTE));
}

u16 ms_from_time(double t)
{
    return static_cast<u16>(fmod(t, MS_PER_SECOND));
}

Value make_time(GlobalObject& global_object, Value hour, Value min, Value sec, Value ms)
{
    if (!hour.is_finite_number() || !min.is_finite_number() || !sec.is_finite_number() || !ms.is_finite_number())
        return js_nan();

    auto h = hour.to_integer_or_infinity(global_object);
    auto m = min.to_integer_or_infinity(global_object);
    auto s = sec.to_integer_or_infinity(global_object);
    auto milli = ms.to_integer_or_infinity(global_object);

    auto t = ((h * MS_PER_HOUR + m * MS_PER_MINUTE) + s * MS_PER_SECOND) + milli;
    return Value(t);
}

double day(double time_value)
{
    return floor(time_value / MS_PER_DAY);
}

Value make_day(GlobalObject& global_object, Value year, Value month, Value date)
{
    if (!year.is_finite_number() || !month.is_finite_number() || !date.is_finite_number())
        return js_nan();

    auto y = year.to_integer_or_infinity(global_object);
    auto m = month.to_integer_or_infinity(global_object);
    auto dt = date.to_integer_or_infinity(global_object);
    auto ym = Value(y + floor(m / 12));
    if (!ym.is_finite_number())
        return js_nan();

    auto t = Core::DateTime::create(y, m + 1, 0).timestamp() * 1000;
    return Value(day(t) + dt - 1);
}

Value make_date(Value day, Value time)
{
    if (!day.is_finite_number() || !time.is_finite_number())
        return js_nan();

    auto tv = Value(day.as_double() * MS_PER_DAY + time.as_double());

    if (!tv.is_finite_number())
        return js_nan();

    return tv;
}

}
