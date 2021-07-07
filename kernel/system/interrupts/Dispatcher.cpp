/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include "system/Streams.h"
#include <assert.h>
#include "system/devices/Devices.h"
#include "system/interrupts/Dispatcher.h"
#include "system/interrupts/Interupts.h"
#include "system/scheduling/Blocker.h"
#include "system/scheduling/Scheduler.h"

static bool _pending_interrupts[256] = {};

void dispatcher_initialize()
{
    Task *task = task_spawn(nullptr, "interrupts-dispatcher", dispatcher_service, nullptr, false);
    task_go(task);
}

void dispatcher_dispatch(int interrupt)
{
    _pending_interrupts[interrupt] = true;
    devices_acknowledge_interrupt(interrupt);
}

static bool dispatcher_has_interrupt()
{
    InterruptsRetainer retainer;

    int result = 0;

    for (size_t i = 0; i < 256; i++)
    {
        if (_pending_interrupts[i])
        {
            result++;
        }
    }

    return result > 0;
}