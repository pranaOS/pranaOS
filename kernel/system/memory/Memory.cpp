/*
 * Copyright (c) 2021, krishpranav, johnsmith5c12
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include <assert.h>
#include <string.h>
#include "archs/Arch.h"
#include "system/Streams.h"
#include "system/graphics/Graphics.h"
#include "system/interrupts/Interupts.h"
#include "system/memory/Memory.h"
#include "system/memory/MemoryObject.h"
#include "system/memory/Physical.h"

static bool _memory_initialized = false;

extern int __start;
extern int __end;

static MemoryRange kernel_memory_range()
{
    return MemoryRange::around_non_aligned_address((uintptr_t)&__start, (size_t)&__end - (size_t)&__start);
}

void memory_initialize(Handover *handover)
{
    Kernel::logln("Initializing memory management...");

    for (size_t i = 0; i < 1024 * 1024 / 8; i++)
    {
        MEMORY[i] = 0xff;
    }

    for (size_t i = 0; i < handover->memory_map_size; i++)
    {
        MemoryMapEntry *entry = &handover->memory_map[i];

        if (entry->type == MEMORY_MAP_ENTRY_AVAILABLE)
        {
            physical_set_free(entry->range);
        }
    }

    Arch::virtual_initialize();

    USED_MEMORY = 0;
    TOTAL_MEMORY = handover->memory_usable;

    Kernel::logln("Mapping kernel...");
    memory_map_identity(Arch::kernel_address_space(), kernel_memory_range(), MEMORY_NONE);

    Kernel::logln("Mapping modules...");
    for (size_t i = 0; i < handover->modules_size; i++)
    {
        memory_map_identity(Arch::kernel_address_space(), handover->modules[i].range, MEMORY_NONE);
    }


    MemoryRange page_zero{0, ARCH_PAGE_SIZE};
    Arch::virtual_free(Arch::kernel_address_space(), page_zero);
    physical_set_used(page_zero);

    Arch::address_space_switch(Arch::kernel_address_space());
    graphic_did_find_framebuffer(0, 0, 0, 0, 0);

    Arch::virtual_memory_enable();

    Kernel::logln("{}Kio of memory detected", TOTAL_MEMORY / 1024);
    Kernel::logln("{}Kio of memory is used by the kernel", USED_MEMORY / 1024);

    Kernel::logln("Paging enabled!");

    _memory_initialized = true;

    memory_object_initialize();
}

void memory_dump()
{
    Kernel::logln("\tMemory Status: ");
    Kernel::logln("\t - Used  physical Memory: {12d}kib", USED_MEMORY / 1024);
    Kernel::logln("\t - Total physical Memory: {12d}kib", TOTAL_MEMORY / 1024);
}

size_t memory_get_used()
{
    InterruptsRetainer retainer;

    return USED_MEMORY;
}

size_t memory_get_total()
{
    InterruptsRetainer retainer;

    return TOTAL_MEMORY;
}

JResult memory_map(Arch::AddressSpace *address_space, MemoryRange virtual_range, MemoryFlags flags)
{
    assert(virtual_range.is_page_aligned());

    InterruptsRetainer retainer;

    for (size_t i = 0; i < virtual_range.size() / ARCH_PAGE_SIZE; i++)
    {
        uintptr_t virtual_address = virtual_range.base() + i * ARCH_PAGE_SIZE;

        if (!Arch::virtual_present(address_space, virtual_address))
        {
            auto physical_range = physical_alloc(ARCH_PAGE_SIZE);
            HjResult virtual_map_result = Arch::virtual_map(address_space, physical_range, virtual_address, flags);

            if (virtual_map_result != SUCCESS)
            {
                return virtual_map_result;
            }
        }
    }

    if (flags & MEMORY_CLEAR)
    {
        memset((void *)virtual_range.base(), 0, virtual_range.size());
    }

    return SUCCESS;
}

JResult memory_map_identity(Arch::AddressSpace *address_space, MemoryRange physical_range, MemoryFlags flags)
{
    assert(physical_range.is_page_aligned());

    InterruptsRetainer retainer;

    physical_set_used(physical_range);
    assert(SUCCESS == Arch::virtual_map(address_space, physical_range, physical_range.base(), flags));

    if (flags & MEMORY_CLEAR)
    {
        memset((void *)physical_range.base(), 0, physical_range.size());
    }

    return SUCCESS;
}