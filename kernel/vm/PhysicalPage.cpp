/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/heap/kmalloc.h>
#include <kernel/vm/MemoryManager.h>
#include <kernel/vm/PhysicalPage.h>

namespace Kernel {

NonnullRefPtr<PhysicalPage> PhysicalPage::create(PhysicalAddress paddr, MayReturnToFreeList may_return_to_freelist)
{
    auto& physical_page_entry = MM.get_physical_page_entry(paddr);
    return adopt_ref(*new (&physical_page_entry.allocated.physical_page) PhysicalPage(may_return_to_freelist));
}

PhysicalPage::PhysicalPage(MayReturnToFreeList may_return_to_freelist)
    : m_may_return_to_freelist(may_return_to_freelist)
{
}

}