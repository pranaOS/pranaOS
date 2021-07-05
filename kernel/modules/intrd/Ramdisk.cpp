/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libabi/Result.h>
#include <libfile/TARArchive.h>
#include "archs/Arch.h"
#include "system/Streams.h"
#include "system/memory/Memory.h"
#include "system/modules/Modules.h"
#include "system/scheduling/Scheduler.h"


void ramdisk_load(Module *module)
{
    TARBlock block;
    for (size_t i = 0; tar_read(void*)module->range.base())
}