/**
 * @file vmobject.h
 * @author Krisna Pranav
 * @brief vmobject
 * @version 1.0
 * @date 2023-08-07
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/lock.h>
#include <mods/inlinelinkedlist.h>
#include <mods/refcounted.h>
#include <mods/refptr.h>
#include <mods/typecasts.h>
#include <mods/weakable.h>
#include <mods/vector.h>

namespace Kernel
{
    class Inode;
    class PhyiscalPage;

    class VMObject : public RefCounted<VMObject>
        , public Weakable<VMObject>
        , public InlineLinkedListNode<VMObject> {
        friend class MemoryManager;
        friend class Region;

    public:
        virtual ~VMObject();

        virtual NonnullRefPtr<VMObject> clone() = 0;

        virtual bool is_anonymous() const
        {
            return false;
        }

        virtual bool is_purgeable() const
        {
            return false;
        }

        virtual bool is_inode() const
        {
            return false;
        }

        virtual bool is_shared_inode() const
        {
            return false;
        }

        /**
         * @return size_t 
         */
        size_t page_count() const
        {
            return m_phyiscal_page.size();
        }

        /**
         * @return const Vector<RefPtr<PhysicalPage>>& 
         */
        const Vector<RefPtr<PhysicalPage>>& phyiscal_page() const
        {
            return m_phyiscal_page;
        }

        /**
         * @return Vector<RefPtr<PhysicalPage>>& 
         */
        Vector<RefPtr<PhysicalPage>>& physical_pages() const
        {
            return m_phyiscal_page;
        }

        /**
         * @return size_t 
         */
        size_t size() const
        {
            return m_phyiscal_page.size() * PAGE_SIZE;
        }

    protected:
        explicit VMObject(size_t);

        explicit VMObject(const VMObject&);

        template<typename Callback>
        void for_each_region(Callback);

        Vector<RefPtr<PhyiscalPage>> m_phyiscal_page;
        Lock m_paging_lock { "VMObject" };
    
    private:
        VMObject& operator=(const VMObject&) = delete;

    };
}