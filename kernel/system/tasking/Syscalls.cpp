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

typedef JResult (*SyscallHandler)(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t);

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

static bool valid_launchpad(Launchpad *launchpad)
{
    return syscall_validate_ptr((uintptr_t)launchpad, sizeof(Launchpad)) &&
           validate_launchpad_arguments(launchpad) &&
           syscall_validate_ptr((uintptr_t)launchpad->env, launchpad->env_size);
}

static Launchpad copy_launchpad(Launchpad *launchpad)
{
    Launchpad launchpad_copy = *launchpad;

    for (int i = 0; i < launchpad->argc; i++)
    {
        launchpad_copy.argv[i].buffer = strdup(launchpad->argv[i].buffer);
        launchpad_copy.argv[i].size = launchpad->argv[i].size;
    }

    launchpad_copy.env = strdup(launchpad->env);
    launchpad_copy.env_size = launchpad->env_size;

    return launchpad_copy;
}

static void free_launchpad(Launchpad *launchpad)
{
    free(launchpad->en);

    for (int i = 0; i < launchpad->argc; i++)
    {
        free(launchpad->argv[i].buffer);
    }
}

JResult J_process_launch(Launchpad *launchpad, int *pid)
{
    if (!valid_launchpad(launchpad) ||
        !syscall_validate_ptr((uintptr_t)pid, sizeof(int)))
    {
        return ERR_BAD_ADDRESS;
    }

    auto launchpad_copy = copy_launchpad(launchpad);

    launchpad_copy.flags |= TASK_USER;

    JResult result = task_launch(scheduler_running(), &launchpad_copy, pid);

    free_launchpad(&launchpad_copy);

    return result;
}


JResult J_process_clone(int *, TaskFlags)
{
    return ERR_NOT_IMPLEMENTED;
}

JResult J_process_exec(Launchpad *launchpad)
{
    if (!valid_launchpad(launchpad))
    {
        return ERR_BAD_ADDRESS;
    }

    auto launchpad_copy = copy_launchpad(launchpad);

    JResult result = task_exec(scheduler_running(), &launchpad_copy);

    free_launchpad(&launchpad_copy);

    return result;
}

JResult J_process_exit(int exit_code)
{
    if (exit_code != PROCESS_SUCCESS)
    {
        Kernel::logln("Process terminated with error code {}!", exit_code);
        Arch::backtrace();
    }

    return scheduler_running()->cancel(exit_code);
}

JResult J_process_cancel(int pid)
{
    InterruptsRetainer retainer;

    Task *task = task_by_id(pid);

    if (task == nullptr)
    {
        return ERR_NO_SUCH_TASK;
    }
    else if (!(task->_flags & TASK_USER))
    {
        return ERR_ACCESS_DENIED;
    }
    else
    {
        return task->cancel(PROCESS_FAILURE);
    }
}

JResult J_process_sleep(int time)
{
    return task_sleep(scheduler_running(), time);
}

JResult J_process_wait(int tid, int *user_exit_value)
{
    int exit_value;

    JResult result = task_wait(tid, &exit_value);

    if (syscall_validate_ptr((uintptr_t)user_exit_value, sizeof(int)))
    {
        *user_exit_value = exit_value;
    }

    return result;
}

JResult J_memory_alloc(size_t size, uintptr_t *out_address)
{
    if (!syscall_validate_ptr((uintptr_t)out_address, sizeof(uintptr_t)))
    {
        return ERR_BAD_ADDRESS;
    }

    return task_memory_alloc(scheduler_running(), size, out_address);
}

JResult J_memory_map(uintptr_t address, size_t size, int flags)
{
    if (!syscall_validate_ptr(address, size))
    {
        return ERR_BAD_ADDRESS;
    }

    return task_memory_map(scheduler_running(), address, size, flags);
}

JResult J_memory_free(uintptr_t address)
{
    return task_memory_free(scheduler_running(), address);
}

JResult J_memory_include(int handle, uintptr_t *out_address, size_t *out_size)
{

    if (!syscall_validate_ptr((uintptr_t)out_address, sizeof(uintptr_t)) ||
        !syscall_validate_ptr((uintptr_t)out_size, sizeof(size_t)))
    {
        return ERR_BAD_ADDRESS;
    }

    return task_memory_include(scheduler_running(), handle, out_address, out_size);
}

JResult J_memory_get_handle(uintptr_t address, int *out_handle)
{
    if (!syscall_validate_ptr((uintptr_t)out_handle, sizeof(int)))
    {
        return ERR_BAD_ADDRESS;
    }

    return task_memory_get_handle(scheduler_running(), address, out_handle);
}