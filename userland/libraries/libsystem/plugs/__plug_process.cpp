/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libabi/Syscalls.h>
#include <assert.h>
#include <libio/Path.h>
#include <libsystem/core/Plugs.h>

int __plug_process_this()
{
    int pid = -1;
    assert(J_process_this(&pid) == SUCCESS);
    return pid;
}

static bool _has_cached_name = false;

static char _cached_name[PROCESS_NAME_SIZE];

const char *__plug_process_name()
{
    if (!_has_cached_name)
    {
        assert(J_process_name(_cached_name, PROCESS_NAME_SIZE) == SUCCESS);
    }

    return _cached_name;
}

JResult __plug_process_launch(Launchpad *launchpad, int *pid)
{
    return J_process_launch(launchpad, pid);
}

void __plug_process_exit(int code)
{
    J_process_exit(code);

    ASSERT_NOT_REACHED();
}

JResult __plug_process_cancel(int pid)
{
    return J_process_cancel(pid);
}

JResult __plug_process_sleep(int time)
{
    return J_process_sleep(time);
}

JResult __plug_process_wait(int pid, int *exit_value)
{
    return J_process_wait(pid, exit_value);
}