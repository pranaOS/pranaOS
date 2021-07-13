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

void Task::state(TaskState state)
{
    ASSERT_INTERRUPTS_RETAINED();

    scheduler_did_change_task_state(this, _state, state);
    _state = state;

    if (state == TASK_STATE_CANCELED)
    {
        _tasks->remove(this);
        Kernel::finalize_task(this);
    }
}

void Task::interrupt()
{
    InterruptsRetainer retainer;

    _is_interrupted = true;

    if (_blocker)
    {
        _blocker->interrupt(*this, INTERRUPTED);
    }
}

JResult Task::cancel(int exit_value)
{
    interrupts_retain();

    if (_is_canceled)
    {
        interrupts_release();
        return SUCCESS;
    }

    _is_canceled = true;
    this->exit_value = exit_value;

    if (_is_doing_syscall)
    {
        interrupts_release();
        interrupt();
        return SUCCESS;
    }

    interrupts_release();
    kill_me_if_you_dare();

    return SUCCESS;
}

void Task::kill_me_if_you_dare()
{
    interrupts_retain();

    if (!_is_canceled)
    {
        interrupts_release();
        return;
    }

    if (_flags & TASK_WAITABLE)
    {
        state(TASK_STATE_CANCELING);
    }
    else
    {
        state(TASK_STATE_CANCELED);
    }

    if (this == scheduler_running())
    {
        interrupts_release();
        scheduler_yield();
        ASSERT_NOT_REACHED();
    }
    else
    {
        interrupts_release();
    }
}