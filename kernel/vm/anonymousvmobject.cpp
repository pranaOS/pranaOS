/**
 * @file anonymousvmobject.cpp
 * @author Krisna Pranav
 * @brief anonymous vm object
 * @version 1.0
 * @date 2023-08-08
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/vm/anonymousvmobject.h>
#include <kernel/vm/physicalpage.h>
#include <kernel/vm/memorymanager.h>
#include <mods/nonnullrefptr.h>


namespace Kernel
{
    /**
     * @param size 
     * @return NonnullRefPtr<AnonymousVMObject> 
     */
    NonnullRefPtr<AnonymousVMObject> AnonymousVMObject::create_with_size(size_t size)
    {
        return adopt(*new AnonymousVMObject(size));
    }

    /**
     * @param paddr 
     * @param size 
     * @return RefPtr<AnonymousVMObject> 
     */
    RefPtr<AnonymousVMObject> AnonymousVMObject::create_for_physical_range(PhysicalAddress paddr, size_t size)
    {
        if (paddr.offset(size) < paddr) {
            dbg() << "create_for_physical_range()";

            return nullptr;
        }
    }

} // namespace Kernel