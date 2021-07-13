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

Task *task_create(Task *parent, const char *name, TaskFlags flags)
{
    UNUSED(parent);

    ASSERT_INTERRUPTS_RETAINED();

    if (_tasks == nullptr)
    {
        _tasks = new List<Task *>();
    }

    Task *task = new Task{};

    task->id = _task_ids++;
    strlcpy(task->name, name, PROCESS_NAME_SIZE);
    task->_state = TASK_STATE_NONE;
    task->_flags = flags;

    if (task->_flags & TASK_USER)
    {
        task->address_space = Arch::address_space_create();
    }
    else
    {
        task->address_space = Arch::kernel_address_space();
    }

    if (parent)
        task->_domain = parent->_domain;

    task->memory_mapping = new List<MemoryMapping *>();

    memory_alloc(task->address_space, PROCESS_STACK_SIZE, MEMORY_CLEAR, (uintptr_t *)&task->kernel_stack);
    task->kernel_stack_pointer = ((uintptr_t)task->kernel_stack + PROCESS_STACK_SIZE);

    if (task->_flags & TASK_USER)
    {
        auto *parent_address_space = task_switch_address_space(scheduler_running(), task->address_space);
        task_memory_map(task, 0xff000000, PROCESS_STACK_SIZE, MEMORY_CLEAR | MEMORY_USER);
        task->user_stack_pointer = 0xff000000 + PROCESS_STACK_SIZE;
        task->user_stack = (void *)0xff000000;
        task_switch_address_space(scheduler_running(), parent_address_space);
    }

    Arch::save_context(task);

    _tasks->push_back(task);

    return task;
}


Task *task_clone(Task *parent, uintptr_t sp, uintptr_t ip, TaskFlags flags)
{
    ASSERT_INTERRUPTS_RETAINED();

    if (_tasks == nullptr)
    {
        _tasks = new List<Task *>();
    }

    Task *task = new Task{};

    task->id = _task_ids++;
    task->_flags = flags;
    strlcpy(task->name, parent->name, PROCESS_NAME_SIZE);
    task->_state = TASK_STATE_NONE;

    task->address_space = Arch::address_space_create();

    task->memory_mapping = new List<MemoryMapping *>();

    if (parent)
    {
        task->_domain = parent->_domain;
    }

    for (int i = 0; i < PROCESS_HANDLE_COUNT; i++)
    {
        parent->handles().pass(task->handles(), i, i);
    }

    memory_alloc(task->address_space, PROCESS_STACK_SIZE, MEMORY_CLEAR, (uintptr_t *)&task->kernel_stack);
    task->kernel_stack_pointer = ((uintptr_t)task->kernel_stack + PROCESS_STACK_SIZE);

    for (auto *mapping : *parent->memory_mapping)
    {
        auto virtual_range = mapping->range();

        auto physical_range = mapping->object->range();

        void *buffer = malloc(physical_range.size());
        assert(buffer);
        assert(virtual_range.base());
        memcpy(buffer, (void *)virtual_range.base(), virtual_range.size());

        auto *parent_address_space = task_switch_address_space(scheduler_running(), task->address_space);

        task_memory_map(task, virtual_range.base(), virtual_range.size(), MEMORY_USER);
        memcpy((void *)virtual_range.base(), buffer, virtual_range.size());

        task_switch_address_space(scheduler_running(), parent_address_space);

        free(buffer);
    }

    task->user_stack_pointer = sp;
    task->entry_point = (TaskEntryPoint)ip;

    _tasks->push_back(task);

    task_go(task);

    return task;
}

void task_destroy(Task *task)
{
    interrupts_retain();

    task->state(TASK_STATE_NONE);

    interrupts_release();

    while (task->memory_mapping->any())
    {
        MemoryMapping *mapping = task->memory_mapping->peek();
        task_memory_mapping_destroy(task, mapping);
    }

    delete task->memory_mapping;

    memory_free(task->address_space, MemoryRange{(uintptr_t)task->kernel_stack, PROCESS_STACK_SIZE});

    if (task->address_space != Arch::kernel_address_space())
    {
        Arch::address_space_destroy(task->address_space);
    }

    delete task;
}

void task_clear_userspace(Task *task)
{
    while (task->memory_mapping->any())
    {
        MemoryMapping *mapping = task->memory_mapping->peek();
        task_memory_mapping_destroy(task, mapping);
    }

    auto *parent_address_space = task_switch_address_space(scheduler_running(), task->address_space);
    task_memory_map(task, 0xff000000, PROCESS_STACK_SIZE, MEMORY_CLEAR | MEMORY_USER);
    task->user_stack_pointer = 0xff000000 + PROCESS_STACK_SIZE;
    task->user_stack = (void *)0xff000000;
    task_switch_address_space(scheduler_running(), parent_address_space);
}

void task_iterate(void *target, TaskIterateCallback callback)
{
    InterruptsRetainer retainer;

    if (!_tasks)
    {
        return;
    }

    _tasks->foreach([&](auto *task) { callback(target, task); return Iteration::CONTINUE; });

}

Task *task_by_id(int id)
{
    for (auto *task : *_tasks)
    {
        if (task->id == id)
        {
            return task;
        }
    }

    return nullptr;
}

