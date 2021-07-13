/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include "system/memory/MemoryObject.h"
#include "system/tasking/Task.h"

struct MemoryObject
{
    MemoryObject *object;

    uintptr_t address;
    size_t size;

    MemoryRange range() { return {address, size}; }
};

MemoryMapping *task_memory_mapping_create(Task *task, MemoryObject *memory_object);

void task_memory_mapping_destroy(Task *task, MemoryObject *memory_mapping);