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
    class PhysicalPage;

    class VMObject : public RefCounted<VMObject>
        , public Weakable<VMObject>
        , public InlineLinkedListNode<VMObject> {
        friend class MemoryManager;
        friend class Region;

    public:
        virtual ~VMObject();

        /**
         * @return NonnullRefPtr<VMObject> 
         */
        virtual NonnullRefPtr<VMObject> clone() = 0;

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_anonymous() const 
        { 
            return false; 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_purgeable() const 
        { 
            return false; 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_inode() const 
        { 
            return false; 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_shared_inode() const 
        { 
            return false; 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_private_inode() const 
        { 
            return false; 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_contiguous() const 
        { 
            return false; 
        }

        /**
         * @return size_t 
         */
        size_t page_count() const 
        { 
            return m_physical_pages.size(); 
        }

        /**
         * @return const Vector<RefPtr<PhysicalPage>>& 
         */
        const Vector<RefPtr<PhysicalPage>>& physical_pages() const 
        { 
            return m_physical_pages; 
        }

        /**
         * @return Vector<RefPtr<PhysicalPage>>& 
         */
        Vector<RefPtr<PhysicalPage>>& physical_pages() 
        { 
            return m_physical_pages; 
        }

        /**
         * @return size_t 
         */
        size_t size() const 
        { 
            return m_physical_pages.size() * PAGE_SIZE; 
        }

        virtual const char* class_name() const = 0;

        VMObject* m_next { nullptr };
        VMObject* m_prev { nullptr };

    protected:
        explicit VMObject(size_t);
        explicit VMObject(const VMObject&);

        template<typename Callback>
        void for_each_region(Callback);

        Vector<RefPtr<PhysicalPage>> m_physical_pages;
        Lock m_paging_lock { "VMObject" };

    private:
        VMObject& operator=(const VMObject&) = delete;
        VMObject& operator=(VMObject&&) = delete;
        VMObject(VMObject&&) = delete;
    };

} // namespace Kernel
