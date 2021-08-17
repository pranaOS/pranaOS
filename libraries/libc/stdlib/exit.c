/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdlib.h>
#include <sysdep.h>

void exit(int status)
{
    DO_SYSCALL_1(SYS_EXIT, status);
    __builtin_unreachable();
}

void abort()
{
    // TODO: Raise signal
    exit(127);
}
