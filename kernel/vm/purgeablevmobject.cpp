/**
 * @file purgeablevmobject.cpp
 * @author Krisna Pranav
 * @brief purgeablevmobject
 * @version 1.0
 * @date 2023-08-09
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include "purgeablevmobject.h"
#include <kernel/vm/memorymanager.h>
#include <kernel/vm/physicalpage.h>
#include <kernel/vm/purgeablevmobject.h>

namespace Kernel
{
    /**
     * @param size 
     * @return NonnullRefPtr<PurgeableVMObject> 
     */
    NonnullRefPtr<PurgeableVMObject> PurgeableVMObject::create_with_size(size_t size)
    {
        return adopt(*new PurgeableVMObject(size));
    }

    /**
     * @param size 
     */
    PurgeableVMObject::PurgeableVMObject(size_t size)
        : AnonymousVMObject(size)
    {}

    /**
     * @return int 
     */
    int PurgeableVMObject::purge()
    {
        LOCKER(m_paging_lock);
        return purge_impl();
    }

    /**
     * @return int 
     */
    int PurgeableVMObject::purge_with_interrupts_disabled(Badge<MemoryManager>)
    {
        ASSERT_INTERRUPTS_DISABLED();

        if (m_paging_lock.is_locked())
            return 0;
        
        return purge_impl();
    }

    /**
     * @return int 
     */
    int PurgeableVMObject::purge_impl()
    {
        if (!m_volatile)
            return 0;
        
        int purged_page_count = 0;

        for (size_t i = 0; i < m_physical_pages.size(); ++i) {
            if (m_physical_pages[i] && !m_physical_pages[i]->is_shared_zero_page()) 
                ++purged_page_count;

            m_physical_pages[i] = MM.shared_zero_page();
        }

        m_was_purged = true;

        return purged_page_count;
    }

} // namespace Kernel