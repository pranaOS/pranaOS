/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include <libabi/Syscalls.h>
#include <assert.h>
#include <libsystem/core/Plugs.h>
#include <libsystem/process/Launchpad.h>

int process_this()
{
    return __plug_process_this();
}

const char *process_name()
{
    return __plug_process_name();
}

JResult process_run(const char *command, int *pid, TaskFlags flags)
{
    Launchpad *launchpad = launchpad_create("shell", "/Applications/shell/sh");

    launchpad_flags(launchpad, flags);
    launchpad_argument(launchpad, "-c");
    launchpad_argument(launchpad, command);

    return launchpad_launch(launchpad, pid);
}

void NO_RETURN process_exit(int code)
{
    __plug_process_exit(code);
    __builtin_unreachable();
}

void NO_RETURN process_abort()
{
    __plug_process_exit(PROCESS_FAILURE);
    __builtin_unreachable();
}

JResult process_cancel(int pid)
{
    return __plug_process_cancel(pid);
}

JResult process_get_directory(char *buffer, size_t size)
{
    return __plug_process_get_directory(buffer, size);
}

JResult process_set_directory(const char *directory)
{
    return __plug_process_set_directory(directory);
}

String process_resolve(String path)
{
    return __plug_process_resolve(path);
}

JResult process_sleep(int time)
{
    return __plug_process_sleep(time);
}

JResult process_wait(int pid, int *exit_value)
{
    return __plug_process_wait(pid, exit_value);
}