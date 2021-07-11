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

}