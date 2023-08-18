/**
 * @file region.h
 * @author Krisna Pranav
 * @brief region
 * @version 6.0
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
            Execute = 4,
        }; // enum Access

        enum class InheritMode 
        {
            Default,
            ZeroedOnFork,
        }; // enum 

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
         * @return true 
         * @return false 
         */
        bool is_executable() const 
        { 
            return m_access & Access::Execute; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_cacheable() const 
        { 
            return m_cacheable; 
        }

        /**
         * @return const String& 
         */
        const String& name() const 
        { 
            return m_name; 
        }

        /**
         * @return unsigned 
         */
        unsigned access() const 
        { 
            return m_access; 
        }

        /**
         * @param name 
         */
        void set_name(const String& name) 
        { 
            m_name = name; 
        }

        /**
         * @param name 
         */
        void set_name(String&& name) 
        { 
            m_name = move(name); 
        }

        /**
         * @return const VMObject& 
         */
        const VMObject& vmobject() const 
        { 
            return *m_vmobject; 
        }

        /**
         * @return VMObject& 
         */
        VMObject& vmobject() 
        { 
            return *m_vmobject; 
        }

        /**
         * @param obj 
         */
        void set_vmobject(NonnullRefPtr<VMObject>&& obj) 
        { 
            m_vmobject = obj; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_shared() const 
        { 
            return m_shared; 
        }

        /**
         * @param shared 
         */
        void set_shared(bool shared) 
        { 
            m_shared = shared; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_stack() const 
        { 
            return m_stack; 
        }

        /**
         * @param stack 
         */
        void set_stack(bool stack) 
        { 
            m_stack = stack; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_mmap() const 
        { 
            return m_mmap; 
        }

        /**
         * @param mmap 
         */
        void set_mmap(bool mmap) 
        { 
            m_mmap = mmap; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_user_accessible() const 
        { 
            return m_user_accessible; 
        }

        /**
         * @param b 
         */
        void set_user_accessible(bool b) { m_user_accessible = b; }

        /**
         * @return true 
         * @return false 
         */
        bool is_kernel() const 
        { 
            return m_kernel || vaddr().get() >= 0xc0000000; 
        }

        /**
         * @param kernel 
         */
        void set_kernel(bool kernel) 
        { 
            m_kernel = kernel; 
        }

        /**
         * @return PageFaultResponse 
         */
        PageFaultResponse handle_fault(const PageFault&);

        /**
         * @return NonnullOwnPtr<Region> 
         */
        NonnullOwnPtr<Region> clone();

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

        /**
         * @return size_t 
         */
        size_t first_page_index() const
        {
            return m_offset_in_vmobject / PAGE_SIZE;
        }

        /**
         * @return size_t 
         */
        size_t last_page_index() const
        {
            return (first_page_index() + page_count()) - 1;
        }

        /**
         * @return size_t 
         */
        size_t page_count() const
        {
            return size() / PAGE_SIZE;
        }

        /**
         * @param index 
         * @return const PhysicalPage* 
         */
        const PhysicalPage* physical_page(size_t index) const
        {
            ASSERT(index < page_count());
            return vmobject().physical_pages()[first_page_index() + index];
        }

        /**
         * @param index 
         * @return RefPtr<PhysicalPage>& 
         */
        RefPtr<PhysicalPage>& physical_page_slot(size_t index)
        {
            ASSERT(index < page_count());
            return vmobject().physical_pages()[first_page_index() + index];
        }

        /**
         * @return size_t 
         */
        size_t offset_in_vmobject() const
        {
            return m_offset_in_vmobject;
        }

        /**
         * @return true 
         * @return false 
         */
        bool commit();

        /**
         * @return size_t 
         */
        size_t amount_resident() const;
        size_t amount_shared() const;
        size_t amount_dirty() const;

        /**
         * @param page_index 
         * @return true 
         * @return false 
         */
        bool should_cow(size_t page_index) const;

        /**
         * @param page_index 
         */
        void set_should_cow(size_t page_index, bool);

        /**
         * @return u32 
         */
        u32 cow_pages() const;

        /**
         * @param b 
         */
        void set_readable(bool b) 
        { 
            set_access_bit(Access::Read, b); 
        }

        /**
         * @param b 
         */
        void set_writable(bool b) 
        { 
            set_access_bit(Access::Write, b); 
        }

        /**
         * @param b 
         */
        void set_executable(bool b) 
        { 
            set_access_bit(Access::Execute, b); 
        }

        void set_page_directory(PageDirectory&);

        bool map(PageDirectory&);

        enum class ShouldDeallocateVirtualMemoryRange 
        {
            No,
            Yes,
        };

        void unmap(ShouldDeallocateVirtualMemoryRange = ShouldDeallocateVirtualMemoryRange::Yes);

        void remap();

        Region* m_next { nullptr };
        Region* m_prev { nullptr };

        /**
         * @param offset_in_vmobject 
         * @param access 
         * @param cacheable 
         * @param kernel 
         */
        Region(const Range&, NonnullRefPtr<VMObject>, size_t offset_in_vmobject, const String&, u8 access, bool cacheable, bool kernel);

        /**
         * @param inherit_mode 
         */
        void set_inherit_mode(InheritMode inherit_mode) 
        { 
            m_inherit_mode = inherit_mode; 
        }

    private:

        /**
         * @return Bitmap& 
         */
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

    /**
     * @param prot 
     * @return unsigned 
     */
    inline unsigned prot_to_region_access_flags(int prot)
    {
        unsigned access = 0;
        if (prot & PROT_READ)
            access |= Region::Access::Read;
        if (prot & PROT_WRITE)
            access |= Region::Access::Write;
        if (prot & PROT_EXEC)
            access |= Region::Access::Execute;
        return access;
    }

}
