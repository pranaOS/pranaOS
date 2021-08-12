/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <errno.h>
#include <sys/times.h>
#include <syscall.h>

clock_t times(struct tms* buf)
{
    int rc = syscall(SC_times, buf);
    __RETURN_WITH_ERRNO(rc, rc, (clock_t)-1);
}
