/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "system/Streams.h"
#include <string.h>
#include "system/modules/Modules.h"


void module_load(Module *module)
{
    if (strcmp("ramdisk", module->command_line) == 0)
    {
        ramdisk_load(module);
    }
    else
    {
        Kernel::logln("Unknow module: '{}'!", module->command_line);
    }
}