/**
 * @file region.h
 * @author Krisna Pranav
 * @brief region
 * @version 1.0
 * @date 2023-08-08
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once
 
#include <kernel/arch/i386/cpu.h>
#include <kernel/heap/slaballocator.h>
#include <kernel/vm/rangeallocator.h>
#include <kernel/vm/vmobject.h>
#include <mods/inlinelinkedlist.h>
#include <mods/string.h>
#include <mods/weakable.h>

namespace Kernel
{
    class Inode;
    class VMObject;

    enum class PageFaultResponse
    {
        ShouldCrash,
        OutOfMemory,
        Continue,
    }; 

    class Region final
        : public InlineLinkedListNode<Region>
        , public Weakable<Region> {

        friend class MemoryManager;

        MAKE_SLAB_ALLOCATED(Region)

    public:
        enum Access 
        {
            Read = 1,
            Write = 2,
            Execute = 3,  
        };

        enum class InheritMode
        {
            Default,
            ZeroedOnFork,
        };

        /**
         * @param offset_in_vmobject 
         * @param name 
         * @param access 
         * @param cacheable 
         * @return NonnullOwnPtr<Region> 
         */
        static NonnullOwnPtr<Region> create_user_accessible(const Range&, NonnullRefPtr<VMObject>, size_t offset_in_vmobject, const StringView& name, u8 access, bool cacheable = true);

        /**
         * @param offset_in_vmobject 
         * @param name 
         * @param access 
         * @param cacheable 
         * @return NonnullOwnPtr<Region> 
         */
        static NonnullOwnPtr<Region> create_kernel_only(const Range&, NonnullRefPtr<VMObject>, size_t offset_in_vmobject, const StringView& name, u8 access, bool cacheable = true);

        ~Region();

        /**
         * @return const Range& 
         */
        const Range& range() const
        {
            return m_range;
        }

        /**
         * @return VirtualAddress 
         */
        VirtualAddress vaddr() const
        {
            return m_range.base();
        }

        /**
         * @return size_t 
         */
        size_t size() const
        {
            return m_range.size();
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_readable() const
        {
            return m_access & Access::Read;
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_writable() const
        {
            return m_access & Access::Write;
        }

        /**
         * @return const String& 
         */
        const String& name() const
        {
            return m_name;
        }

        /**
         * @param vaddr 
         * @return true 
         * @return false 
         */
        bool contains(VirtualAddress vaddr) const
        {
            return m_range.contains(vaddr);
        }

        /**
         * @param range 
         * @return true 
         * @return false 
         */
        bool contains(const Range& range) const
        {
            return m_range.contains(range);
        }

        /**
         * @param vaddr 
         * @return unsigned 
         */
        unsigned page_index_from_address(VirtualAddress vaddr) const
        {
            return (vaddr - m_range.base()).get() / PAGE_SIZE;
        }

        /**
         * @param page_index 
         * @return VirtualAddress 
         */
        VirtualAddress vaddr_from_page_index(size_t page_index) const
        {
            return vaddr().offset(page_index * PAGE_SIZE);
        }

    private:
        Bitmap& ensure_cow_map() const;

        /**
         * @param access 
         * @param b 
         */
        void set_access_bit(Access access, bool b)
        {
            if (b)  
                m_access |= access;
            else
                m_access &= ~access;
        }

        /**
         * @param page_index 
         * @return true 
         * @return false 
         */
        bool commit(size_t page_index);

        /**
         * @param index 
         * @param with_flush 
         * @return true 
         * @return false 
         */
        bool remap_page(size_t index, bool with_flush = true);

        /**
         * @param page_index 
         * @return PageFaultResponse 
         */
        PageFaultResponse handle_cow_fault(size_t page_index);

        /**
         * @param page_index 
         * @return PageFaultResponse 
         */
        PageFaultResponse handle_inode_fault(size_t page_index);

        /**
         * @param page_index 
         * @return PageFaultResponse 
         */
        PageFaultResponse handle_zero_fault(size_t page_index);

        /**
         * @param page_index 
         * @return true 
         * @return false 
         */
        bool map_individual_page_impl(size_t page_index); 

        RefPtr<PageDirectory> m_page_directory;
        Range m_range;
        size_t m_offset_in_vmobject { 0 };
        NonnullRefPtr<VMObject> m_vmobject;
        String m_name;
        u8 m_access { 0 };
        InheritMode m_inherit_mode : 3 { InheritMode::Default };
        bool m_shared : 1 { false };
        bool m_user_accessible : 1 { false };
        bool m_cacheable : 1 { false };
        bool m_stack : 1 { false };
        bool m_mmap : 1 { false };
        bool m_kernel : 1 { false };
        mutable OwnPtr<Bitmap> m_cow_map;
    };
} // namespace Kernel   