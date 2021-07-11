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
