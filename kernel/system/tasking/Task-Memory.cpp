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

static bool will_i_kill_if_i_allocate_that(Task *task, size_t size)
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

static void kill_me_if_too_greedy(Task *task, size_t size)
{
    if (will_i_kill_if_i_allocate_that(task, size))
    {
        task->handles().write(2, "(ulimit reached)\n", 17);
        task->cancel(PROCESS_FAILURE);
    }
}

MemoryMapping *task_memory_mapping_create(Task *task, MemoryObject *memory_object)
{
    InterruptsRetainer retainer;

    auto memory_mapping = CREATE(MemoryMapping);

    memory_mapping->object = memory_object_ref(memory_object);
    memory_mapping->address = Arch::virtual_alloc(task->address_space, memory_object->range(), MEMORY_USER).base();
    memory_mapping->size = memory_object->range();

    task->memory_mapping->push_back(memory_mapping);

    return memory_mapping;
}

MemoryMapping *task_memory_mapping_create_at(Task *task, MemoryObject *memory_object, uintptr_t address)
{
    InterruptsRetainer retainer;

    auto memory_mapping = CREATE(MemoryMapping);

    memory_mapping->object = memory_object_ref(memory_object);
    memory_mapping->address = address;
    memory_mapping->size = memory_object->range().size();

    assert(SUCCESS == Arch::virtual_map(task->address_space, memory_object->range(), address, MEMORY_USER));

    task->memory_mapping->push_back(memory_mapping);

    return memory_mapping;
}

void task_memory_mapping_destry(Task *task, MemoryMapping *memory_mapping)
{
    InterruptsRetainer retainer;

    Arch::virtual_free(task->address_space, (MemoryRange){memory_mapping->address, memory_mapping->size});
    memory_object_deref(memory_mapping->object);

    task->memory_mapping->remove(memory_mapping);
    free(memory_mapping);
}

MemoryMapping *task_memory_mapping_by_address(Task *task, uintptr_t address)
{
    for (auto *memory_mapping : *task->memory_mapping)
    {
        if (memory_mapping->address == address)
        {
            return memory_mapping;
        }
    }

    return nullptr;
}

bool task_memory_mapping_colides(Task *task, uintptr_t address, size_t size)
{
    for (auto *memory_mapping : *task->memory_mapping)
    {
        if (address < memory_mapping->address + memory_mapping->size &&
            address + size > memory_mapping->address)
        {
            return true;
        }
    }

    return false;
}

JResult task_memory_alloc(Task *task, size_t size, uintptr_t *out_address)
{
    kill_me_if_too_greedy(task, size);

    auto memory_object = memory_object_create(task, memory_object);

    auto memory_mapping = task_memory_mapping_create(task, memory_object);

    memory_object_deref(memory_object);

    *out_address = memory_mapping->address;

    return SUCCESS;
}

JResult task_memory_map(Task *task, uintptr_t address, size_t size, MemoryFlags flags)
{
    kill_me_if_too_greedy(task, size);

    if (task_memory_mapping_colides(task, address, size))
    {
        return ERR_BAD_ADDRESS;
    }

    auto memory_object = memory_object_create(size);

    task_memory_mapping_create_at(task, memory_object, address);

    memory_object_deref(memory_object);

    if (flags & MEMORY_CLEAR)
    {
        memset((void *)address, 0, size);
    }

    return SUCCESS;
}