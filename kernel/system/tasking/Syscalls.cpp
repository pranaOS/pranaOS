/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <assert.h>
#include <string.h>
#include "system/Streams.h"
#include <libabi/Result.h>
#include <libsystem/BuildInfo.h>
#include "archs/Arch.h"
#include "system/interrupts/Interupts.h"
#include "system/scheduling/Scheduler.h"
#include "system/system/System.h"
#include "system/tasking/Syscalls.h"
#include "system/tasking/Task-Launchpad.h"
#include "system/tasking/Task-Memory.h"

typedef HjResult (*SyscallHandler)(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t);

bool syscall_validate_ptr(uintptr_t ptr, size_t size)
{
    return ptr >= 0x100000 && ptr + size >= 0x100000 && ptr + size >= ptr;
}

JResult J_process_this(int *pid)
{
    if (!syscall_validate_ptr((uintptr_t)pid, sizeof(int)))
    {
        return ERR_BAD_ADDRESS;
    }

    *pid = scheduler_running_id();

    return SUCCESS;
}

JResult J_process_name(char *name, size_t size)
{
    if (!syscall_validate_ptr((uintptr_t)name, size))
    {
        return ERR_BAD_ADDRESS;
    }

    strlcpy(name, scheduler_running()->name, size);

    return SUCCESS;
}

static bool validate_launchpad_arguments(Launchpad *launchpad)
{
    for (int i = 0; i < launchpad->argc; i++)
    {
        auto &arg = launchpad->argv[i];

        if (!syscall_validate_ptr((uintptr_t)arg.buffer, arg.size))
        {
            return false;
        }
    }

    return true;
}
