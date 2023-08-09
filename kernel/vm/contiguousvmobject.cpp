/**
 * @file contiguousvmobject.cpp
 * @author Krisna Pranav
 * @brief contiguousvmobject
 * @version 1.0
 * @date 2023-08-09
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/vm/memorymanager.h>
#include <kernel/vm/contiguousvmobject.h>
#include <kernel/vm/physicalpage.h>

namespace Kernel
{
    /**
     * @param size 
     * @return NonnullRefPtr<ContiguousVMObject> 
     */
    NonnullRefPtr<ContiguousVMObject> ContiguousVMObject::create_with_size(size_t size)
    {
        return adopt(*new ContiguousVMObject(size));
    }

    /**
     * @param size 
     */
    ContiguousVMObject::ContiguousVMObject(size_t size)
        : VMObject(size)
    {
        auto contiguous_physical_pages = MM.allocate_contiguous_supervisor_physical_pages(size);

        for (size_t i = 0; i < page_count(); i++) {
            physical_pages()[i] = contiguous_physical_pages[i];
        }
    }

}