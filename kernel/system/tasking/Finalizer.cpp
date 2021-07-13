/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "system/tasking/Finalizer.h"
#include "system/interrupts/Interupts.h"
#include "system/scheduling/Scheduler.h"

namespace Kernel
{

static List<Task *> *_task_to_finalize = nullptr;

void finalize_task(Task *task)
{
    ASSERT_INTERRUPTS_RETAINED();

    assert(_task_to_finalize != nullptr);
    _task_to_finalize->push_back(task);
}

Task *finalizer_pop_task()
{
    InterruptsRetainer retainer
    if (_task_to_finalize->any())
    {
        return _task_to_finalize->pop();
    }
    else
    {
        return nullptr;
    }
}

}