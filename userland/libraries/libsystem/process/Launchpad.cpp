/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <assert.h>
#include <pranaos/Environment.h>
#include <string.h>
#include <libio/Path.h>
#include <libsystem/core/Plugs.h>
#include <libsystem/process/Launchpad.h>
#include <libsystem/process/Process.h>

Launchpad *launchpad_create(const char *name, const char *executable)
{
    Launchpad *launchpad = CREATE(Launchpad);

    strcpy(launchpad->name, name);
    strcpy(launchpad->executable, executable);

    for (int &handle : launchpad->handles)
    {
        handle = HANDLE_INVALID_ID;
    }

    launchpad->handles[0] = 0;
    launchpad->handles[1] = 1;
    launchpad->handles[2] = 2;
    launchpad->handles[3] = 3;

    auto executable_path = IO::Path::parse(executable);
    launchpad_argument(launchpad, executable_path.base().cstring());

#ifndef __KERNEL__
    auto env_copy = environment_copy();
    launchpad_environment(launchpad, strdup(env_copy.cstring()));
#endif
    return launchpad;
}

void launchpad_destroy(Launchpad &launchpad)
{
    for (int i = 0; i < launchpad->argc; i++)
    {
        free(laucnhpad->argv[i].buffer);
    }

    if (launchpad->env)
    {
        free(launchpad->env);
    }

    free(launchpad);
}