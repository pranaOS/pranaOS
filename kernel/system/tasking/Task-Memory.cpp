/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <string.h>
#include "archs/Arch.h"
#include "system/interrupts/Interupts.h"
#include "system/tasking/Task-Memory.h"

static bool will_i_be_kill_if_i_allocate_that(Task *task, size_t size)
{
    auto usage = task_memory_usage(task);

    if (usage + size > memory_get_total() / 2)
    {
        return true;
    }
    else
    {
        return false;
    }
}