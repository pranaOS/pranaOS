/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <assert.h>
#include "system/devices/Devices.h"
#include "system/devices/Driver.h"
#include "system/graphics/Graphics.h"
#include "system/interrupts/Interupts.h"
#include "system/modules/Modules.h"
#include "system/scheduling/Scheduler.h"
#include "system/storage/Partitions.h"
#include "system/system/System.h"
#include "system/tasking/Tasking.h"
#include "system/tasking/Userspace.h"
#include "system/Streams.h"
#include "devfs/DevicesFileSystem.h"
#include "devfs/DevicesInfo.h"
#include "procfs/ProcessInfo.h"

static void splash_screen()
{
    Kernel::logln("");
    Kernel::logln("\u001b[34;1m--------------------------------------------------------------------------------\e[0m");
    Kernel::logln("\u001b[34;1m                             PRANAOS                                            \e[0m");
    Kernel::logln("\u001b[34;1m--------------------------------------------------------------------------------\e[0m");
    Kernel::logln("                              Copyright (c) 2020-2021 The pranaOS contributors ");
    Kernel::logln("");
}

void system_main(Handover *handover)
{
    splash_screen();
    system_initialize();
    memory_initialize(handover);
    scheduler_initialize();
    tasking_initialize();
    interrupts_initialize();
    modules_initialize(handover);
    driver_initialize();
    device_initialize();
    partitions_initialize();
    process_info_initialize();
    device_info_initialize();
    devices_filesystem_initialize();
    graphic_initialize(handover);
    userspace_initialize();

    ASSERT_NOT_REACHED();
}
