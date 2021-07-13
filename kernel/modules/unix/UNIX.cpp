/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "unix/UNIX.h"

Iteration unix_scan(IterFunc<UNIXAddress> callback)
{
    if (callback(UNIX_ZERO) == Iteration::STOP)
    {
        return Iteration::STOP;
    }

    if (callback(UNIX_NULL) == Iteration::STOP)
    {
        return Iteration::STOP;
    }

    if (callback(UNIX_RANDOM) == Iteration::STOP)
    {
        return Iteration::STOP;
    }

    return Iteration::CONTINUE;
}