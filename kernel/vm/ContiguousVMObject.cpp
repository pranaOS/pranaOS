/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/vm/ContiguousVMObject.h>
#include <kernel/vm/MemoryManager.h>
#include <kernel/vm/PhysicalPage.h>

namespace Kernel {

RefPtr<ContiguousVMObject> ContiguousVMObject::try_create_with_size(size_t size)
{
    auto contiguous_physical_pages = MM.allocate_contiguous_supervisor_physical_pages(size);
    if (contiguous_physical_pages.is_empty())
        return {};
    return adopt_ref_if_nonnull(new (nothrow) ContiguousVMObject(size, contiguous_physical_pages));
}

ContiguousVMObject::ContiguousVMObject(size_t size, NonnullRefPtrVector<PhysicalPage>& contiguous_physical_pages)
    : VMObject(size)
{
    for (size_t i = 0; i < page_count(); i++) {
        physical_pages()[i] = contiguous_physical_pages[i];
        dbgln_if(CONTIGUOUS_VMOBJECT_DEBUG, "Contiguous page[{}]: {}", i, physical_pages()[i]->paddr());
    }
}

ContiguousVMObject::ContiguousVMObject(ContiguousVMObject const& other)
    : VMObject(other)
{
}

}