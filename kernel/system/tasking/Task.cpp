/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <assert.h>
#include <string.h>
#include "archs/Arch.h"
#include "system/Streams.h"
#include "system/interrupts/Interupts.h"
#include "system/scheduling/Scheduler.h"
#include "system/system/System.h"
#include "system/tasking/Finalizer.h"
#include "system/tasking/Task-Memory.h"
#include "system/tasking/Task.h"

static int _task_ids = 0;
static List<Task *> *_tasks;

TaskState Task::state()
{
    return _state;
}