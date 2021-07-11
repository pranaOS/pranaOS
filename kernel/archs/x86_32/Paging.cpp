/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <string.h>
#include "system/Streams.h"
#include <libutils/ResultOr.h>
#include "archs/Arch.h"
#include "archs/x86_32/Paging.h"
#include "system/interrupts/Interupts.h"
#include "system/memory/Memory.h"
#include "system/memory/Physical.h"
#include "system/system/System.h"

namespace Arch::x86_32
{

PageDirectory _kernel_page_directory ALIGNED(ARCH_PAGE_SIZE) = {};
PageTable _kernel_page_tables[256] ALIGNED(ARCH_PAGE_SIZE) = {};

void virtual_initialize()
{

    for (size_t i = 0; i < 256; i++)
    {
        PageDirectoryEntry *entry = &_kernel_page_directory.entries[i];
        entry->User = 0;
        entry->Write = 1;
        entry->Present = 1;
        entry->PageFrameNumber = (size_t)&_kernel_page_tables[i] / ARCH_PAGE_SIZE;
    }
}

void virtual_memory_enable()
{
    paging_enable();
}

PageDirectory *kernel_page_directory()
{
    return &_kernel_page_directory;
}

bool virtual_present(PageDirectory *page_directory, uintptr_t virtual_address)
{
    ASSERT_INTERRUPTS_RETAINED();

    int page_directory_index = PAGE_DIRECTORY_INDEX(virtual_address);
    PageDirectoryEntry &page_directory_entry = page_directory->entries[page_directory_index];

    if (!page_directory_entry.Present)
    {
        return false;
    }

    PageTable &page_table = *reinterpret_cast<PageTable *>(page_directory_entry.PageFrameNumber * ARCH_PAGE_SIZE);

    int page_table_index = PAGE_TABLE_INDEX(virtual_address);
    PageTableEntry &page_table_entry = page_table.entries[page_table_index];

    return page_table_entry.Present;
}

}