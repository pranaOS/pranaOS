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
            dbg() << "create for physical range (" << paddr << ", " << size << ") would wrap around";
            return nullptr;
        }
        
        return adopt(*new AnonymousVMObject(paddr, size));
    }

    /**
     * @param page 
     * @return NonnullRefPtr<AnonymousVMObject> 
     */
    NonnullRefPtr<AnonymousVMObject> AnonymousVMObject::create_with_physical_page(PhysicalPage& page)
    {
        auto vmobject = create_with_size(PAGE_SIZE);

        vmobject->m_physical_pages[0] = page;
        return vmobject;
    }

    /**
     * @param size 
     */
    AnonymousVMObject::AnonymousVMObject(size_t size)
        : VMObject(size)
    {
    #ifndef MAP_SHARED_ZERO_PAGE_LAZILY
        for (size_t i = 0; i < page_count(); ++i)
            physical_pages()[i] = MM.shared_zero_page();
    #endif
    }

    /**
     * @param paddr 
     * @param size 
     */
    AnonymousVMObject::AnonymousVMObject(PhysicalAddress paddr, size_t size)
        : VMObject(size)
    {
        ASSERT(paddr.page_base() == paddr);

        for (size_t i = 0; i < page_count(); ++i)
            physical_pages()[i] = PhysicalPage::create(paddr.offset(i * PAGE_SIZE), false, false);
    }

    /**
     * @brief Construct a new Anonymous VM Object::Anonymous VM Object object
     * 
     * @param other 
     */
    AnonymousVMObject::AnonymousVMObject(const AnonymousVMObject& other)
        : VMObject(other)
    { }

    /**
     * @brief Destroy the Anonymous VM Object::Anonymous VM Object object
     * 
     */
    AnonymousVMObject::~AnonymousVMObject()
    {
    }

    /**
     * @return NonnullRefPtr<VMObject> 
     */
    NonnullRefPtr<VMObject> AnonymousVMObject::clone()
    {
        return adopt(*new AnonymousVMObject(*this));
    }

} // namespace Kernel
