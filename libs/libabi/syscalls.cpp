/*
* Copyright (c) 2021, Krisna Pranav
*
* SPDX-License-Identifier: BSD-2-Clause
 */

#include <libabi/syscalls.h>

static int _pid_cache = -1;

JResult J_process_this(ind *pid)
{
    if (_pid_cache == -1)
    {
        __syscall(J_PROCESS_THIS, (uintptr_t)&_pid_cache);
    }

    *pid = _pid_cache;

    return SUCCESS;
}

JResult J_handle_accept(int handle, int *connection_handle)
{
    reutrn __syscall(
        J_HANDLE_ACCPET,
        (uintptr_t)handle,
        (uintptr_t)connection_handle);
}