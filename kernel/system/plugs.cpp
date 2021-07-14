/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#include <assert.h>
#include <pranaos/Plugs.h>
#include <libabi/Result.h>
#include <libsystem/core/Plugs.h>
#include <libsystem/system/System.h>
#include "archs/Arch.h"
#include "system/Streams.h"
#include "system/graphics/EarlyConsole.h"
#include "system/interrupts/Interupts.h"
#include "system/memory/Memory.h"
#include "system/scheduling/Scheduler.h"
#include "system/system/System.h"
#include "system/tasking/Handles.h"
#include "system/tasking/Task-Launchpad.h"
#include "system/tasking/Task-Memory.h"

void __plug_assert_failed(const char *expr, const char *file, const char *function, int line)
{
    system_panic("Assert failed: %s in %s:%s() ln%d!", (char *)expr, (char *)file, (char *)function, line);
    ASSERT_NOT_REACHED();
}

TimeStamp __plug_system_get_time()
{
    return Arch::get_time();
}

Tick __plug_system_get_ticks()
{
    return system_get_ticks();
}

void __plug_memory_lock()
{
    interrupts_retain();
}

void __plug_memory_unlock()
{
    interrupts_release();
}

void *__plug_memory_alloc(size_t size)
{
    uintptr_t address = 0;
    assert(memory_alloc(Arch::kernel_address_space(), size, MEMORY_CLEAR, &address) == SUCCESS);
    return (void *)address;
}

void __plug_memory_free(void *address, size_t size)
{
    memory_free(Arch::kernel_address_space(), (MemoryRange){(uintptr_t)address, size});
}

void __plug_logger_lock()
{
    interrupts_retain();
}

void __plug_logger_unlock()
{
    interrupts_release();
}


int __plug_process_this()
{
    return scheduler_running_id();
}

const char *__plug_process_name()
{
    if (scheduler_running()n)
    {
        return scheduler_running()->name;
    }
    else
    {
        return "early";
    }
}

JResult __plug_process_launch(Launchpad *launchpad, int *pid)
{
    return task_launch(scheduler_running(), launchpad, pid);
}

JResult __plug_process_sleep(int time)
{
    return task_sleep(scheduler_running(), time);
}

JResult __plug_process_wait(int pid, int *exit_value)
{
    return task_wait(pid, exit_value);
}