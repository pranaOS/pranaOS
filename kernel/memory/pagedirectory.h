/**
 * @file pagedirectory.h
 * @author Krisna Pranav
 * @brief Page Directory
 * @version 6.0
 * @date 2024-10-20
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/hashmap.h>
#include <mods/intrusiverbtree.h>
#include <mods/refcounted.h>
#include <mods/refptr.h>
#include <kernel/forward.h>
#include <kernel/memory/physicalpage.h>
#include <kernel/memory/virtualrangeallocator.h>

namespace Kernel::Memory 
{
    class PageDirectory : public RefCounted<PageDirectory> 
    {
        friend class MemoryManager;

    public:
        /**
         * @param parent_range_allocator 
         * @return ErrorOr<NonnullRefPtr<PageDirectory>> 
         */
        static ErrorOr<NonnullRefPtr<PageDirectory>> try_create_for_userspace(VirtualRangeAllocator const* parent_range_allocator = nullptr);

        static NonnullRefPtr<PageDirectory> must_create_kernel_page_directory();

        /**
         * @return RefPtr<PageDirectory> 
         */
        static RefPtr<PageDirectory> find_by_cr3(FlatPtr);

        /**
         * @brief Destroy the Page Directory object
         * 
         */
        ~PageDirectory();

        void allocate_kernel_directory();

        /**
         * @return FlatPtr 
         */
        FlatPtr cr3() const
        {
    #if ARCH(X86_64)
            return m_pml4t->paddr().get();
    #else
            return m_directory_table->paddr().get();
    #endif
        }

        bool is_cr3_initialized() const
        {
    #if ARCH(X86_64)
            return m_pml4t;
    #else
            return m_directory_table;
    #endif
        }

        /**
         * @return VirtualRangeAllocator& 
         */
        VirtualRangeAllocator& range_allocator() 
        { 
            return m_range_allocator; 
        }

        VirtualRangeAllocator const& range_allocator() const 
        { 
            return m_range_allocator; 
        }

        /**
         * @return AddressSpace* 
         */
        AddressSpace* address_space() 
        { 
            return m_space; 
        }

        /**
         * @return const AddressSpace* 
         */
        const AddressSpace* address_space() const 
        { 
            return m_space; 
        }

        /**
         * @brief Set the space object
         * 
         * @param space 
         */
        void set_space(Badge<AddressSpace>, AddressSpace& space) 
        { 
            m_space = &space; 
        }

        /**
         * @brief Get the lock object
         * 
         * @return RecursiveSpinlock& 
         */
        RecursiveSpinlock& get_lock() 
        { 
            return m_lock; 
        }

        IntrusiveRedBlackTreeNode<FlatPtr, PageDirectory, RawPtr<PageDirectory>> m_tree_node;

    private:
        PageDirectory();
        
        AddressSpace* m_space { nullptr };
        VirtualRangeAllocator m_range_allocator;

    #if ARCH(X86_64)
        RefPtr<PhysicalPage> m_pml4t;
    #endif
        RefPtr<PhysicalPage> m_directory_table;
    #if ARCH(X86_64)
        RefPtr<PhysicalPage> m_directory_pages[512];
    #else
        RefPtr<PhysicalPage> m_directory_pages[4];
    #endif
        RecursiveSpinlock m_lock;
    }; // class PageDirectory : public RefCounted<PageDirectory> 
} // namespace Kernel::Memory