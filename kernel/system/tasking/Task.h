/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libabi/Process.h>
#include <libabi/Task.h>
#include <libio/Path.h>
#include <libutils/List.h>
#include "system/memory/Memory.h"
#include "system/scheduling/Blocker.h"
#include "system/tasking/Domain.h"
#include "system/tasking/Handles.h"

typedef void (*TaskEntryPoint)();

struct MemoryMapping;

struct Task
{
    int id;
    char name[PROCESS_NAME_SIZE];
    TaskFlags _flags;

    Syscall _current_syscall;
    bool _is_doing_syscall = false;
    bool _is_canceled = false;
    bool _is_interrutped = false;

    TaskState _state;
    Blocker *_blocker;

    uintptr_t user_stack_pointer;
    void *user_stack;

};