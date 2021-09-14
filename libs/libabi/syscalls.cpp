/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include <libabi/syscalls.h>

static int _pid_cache = -1;

JResult j_process_this(int *pid)
{
    if (_pid_cache == -1)
    {
        __syscall(J_PROCESS_THIS, (uintptr_t)&_pid_cache);
    }

    *pid = _pid_cache;

    return SUCCESS;
}

JResult j_filesystem_link(const char *raw_old_path, size_t old_size, const char *raw_new_path, size_t new_size)
{
    return __syscall(
            J_FILESYSTEM_LINK,
            (uintptr_t)raw_old_path,
            (uintptr_t)old_size,
            (uintptr_t)raw_new_path,
            (uintptr_t)new_size);
}

JRESULT j_handle_stat(int handle, JStat *state)
{
    return __syscall(J_HANDLE_STAT, (uintptr_t)handle, (uintptr_t)state);
}

JResult j_handle_connect(int *handle, const char *raw_path, size_t size)
{
    return __syscall(J_HANDLE_CONNECT, (uintptr_t)handle, (uintptr_t)raw_path, (uintptr_t)size);
}