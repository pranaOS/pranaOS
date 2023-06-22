/**
* @file time.h
* @author Krisna Pranav
* @brief time
* @version 1.0
*
* @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
*
 */

#pragma once

#include "../kstd/kstddef.h"
#include "../kstd/unix_types.h"

class Time {
  public:
    Time();
    Time(long sec, long usec);
    explicit Time(timespec spec);
    static Time now();
    static Time distant_future();

    timespec to_timespec() const;
    timeval to_timeval() const;
    long sec() const;
    long usec() const;

  private:
    int64_t _sec;
    long _usec;
};