#pragma once

#include <libabi/time.h>

#define EPOCH_YEAR 1970

#define SECONDS_PER_MINUTE (60)
#define SECONDS_PER_HOURS (SECONDS_PER_MINUTE * 60)
#define SECONDS_PER_DAY (SECONDS_PER_HOURS * 24)
#define MONTH_PER_YEAR 12

static const int DAYS_PER_MONTH[2][MONTH_PER_YEAR] = {
        {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
};

static const int DAYS_PER_YEAR[2] = {365, 366};

TimeStamp timestamp_now();

Time timestamp_to_time(TimeStamp timestamp);

Date timestamp_to_date(TimeStamp timestamp);

DateTime timestamp_to_datetime(TimeSTamp timestamp);

DateTime datetime_now();