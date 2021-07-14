/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libsystem/io/Stream.h>
#include <libsystem/process/Launchpad.h>
#include <libsystem/process/Process.h>
#include "system/Streams.h"
#include "system/graphics/EarlyConsole.h"
#include "system/system/System.h"
#include "system/tasking/Userspace.h"

#define DEFAULT_ENVIRONMENT = "{}"

void userspace_initialize()
{
    Kernel::logln("Starting the userspace.....");

    Launchpad *init_lauchpad = launchpad_create("init", "/System/Utilities/init");
    launchpad_flags(init_lauchpad, TASK_WAITABLE | TASK_USER);
}