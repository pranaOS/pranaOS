/**
 * @file memorymanager.h
 * @author Krisna Pranav
 * @brief memory_manager
 * @version 6.0
 * @date 2023-08-08
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "vmobject.h"
#include <kernel/forward.h>
#include <kernel/spinlock.h>
#include <kernel/arch/i386/cpu.h>
#include <kernel/vm/physicalpage.h>
#include <kernel/vm/region.h>
#include <kernel/vm/vmobject.h>
#include <mods/hashtable.h>
#include <mods/nonnullrefptrvector.h>
#include <mods/string.h>

namespace Kernel 
{

    #define PAGE_ROUND_UP(x) ((((u32)(x)) + PAGE_SIZE - 1) & (~(PAGE_SIZE - 1)))

    /**
     * @tparam T 
     * @param physical 
     * @return T* 
     */
    template<typename T>
    inline T* low_physical_to_virtual(T* physical)
    {
        return (T*)(((u8*)physical) + 0xc0000000);
    }

    /**
     * @param physical 
     * @return u32 
     */
    inline u32 low_physical_to_virtual(u32 physical)
    {
        return physical + 0xc0000000;
    }
    
    /**
     * @tparam T 
     * @param physical 
     * @return T* 
     */
    template<typename T>
    inline T* virtual_to_low_physical(T* physical)
    {
        return (T*)(((u8*)physical) - 0xc0000000);
    }

    /**
     * @param physical 
     * @return u32 
     */
    inline u32 virtual_to_low_physical(u32 physical)
    {
        return physical - 0xc0000000;
    }

    class KBuffer;
    class SynthFSInode;

    #define MM Kernel::MemoryManager::the()

    struct MemoryManagerData 
    {
        SpinLock<u8> m_quickmap_in_use;
        u32 m_quickmap_prev_flags;

        PhysicalAddress m_last_quickmap_pd;
        PhysicalAddress m_last_quickmap_pt;
    };

    extern RecursiveSpinLock s_mm_lock;

    class MemoryManager 
    {
        MOD_MAKE_ETERNAL
        friend class PageDirectory;
        friend class PhysicalPage;
        friend class PhysicalRegion;
        friend class Region;
        friend class VMObject;
        friend Optional<KBuffer> procfs$mm(InodeIdentifier);
        friend Optional<KBuffer> procfs$memstat(InodeIdentifier);

    public:

        static MemoryManager& the();
        static bool is_initialized();

        /**
         * @param cpu 
         */
        static void initialize(u32 cpu);

        /**
         * @return MemoryManagerData& 
         */
        static inline MemoryManagerData& get_data()
        {
            return Processor::current().get_mm_data();
        }

        /**
         * @return PageFaultResponse 
         */
        PageFaultResponse handle_page_fault(const PageFault&);

        void enter_process_paging_scope(Process&);

        /**
         * @return true 
         * @return false 
         */
        bool validate_user_stack(const Process&, VirtualAddress) const;

        enum class ShouldZeroFill 
        {
            No,
            Yes
        }; // enum

        /**
         * @param did_purge 
         * @return RefPtr<PhysicalPage> 
         */
        RefPtr<PhysicalPage> allocate_user_physical_page(ShouldZeroFill = ShouldZeroFill::Yes, bool* did_purge = nullptr);

        /**
         * @return RefPtr<PhysicalPage> 
         */
        RefPtr<PhysicalPage> allocate_supervisor_physical_page();

        /**
         * @param size 
         * @return NonnullRefPtrVector<PhysicalPage> 
         */
        NonnullRefPtrVector<PhysicalPage> allocate_contiguous_supervisor_physical_pages(size_t size);

        void deallocate_user_physical_page(const PhysicalPage&);
        void deallocate_supervisor_physical_page(const PhysicalPage&);

        /**
         * @param name 
         * @param access 
         * @param user_accessible 
         * @param cacheable 
         * @return OwnPtr<Region> 
         */
        OwnPtr<Region> allocate_contiguous_kernel_region(size_t, const StringView& name, u8 access, bool user_accessible = false, bool cacheable = true);

        /**
         * @param name 
         * @param access 
         * @param user_accessible 
         * @param should_commit 
         * @param cacheable 
         * @return OwnPtr<Region> 
         */
        OwnPtr<Region> allocate_kernel_region(size_t, const StringView& name, u8 access, bool user_accessible = false, bool should_commit = true, bool cacheable = true);

        /**
         * @param name 
         * @param access 
         * @param user_accessible 
         * @param cacheable 
         * @return OwnPtr<Region> 
         */
        OwnPtr<Region> allocate_kernel_region(PhysicalAddress, size_t, const StringView& name, u8 access, bool user_accessible = false, bool cacheable = true);

        /**
         * @param name 
         * @param access 
         * @param user_accessible 
         * @param cacheable 
         * @return OwnPtr<Region> 
         */
        OwnPtr<Region> allocate_kernel_region_identity(PhysicalAddress, size_t, const StringView& name, u8 access, bool user_accessible = false, bool cacheable = true);

        /**
         * @param name 
         * @param access 
         * @param user_accessible 
         * @param cacheable 
         * @return OwnPtr<Region> 
         */
        OwnPtr<Region> allocate_kernel_region_with_vmobject(VMObject&, size_t, const StringView& name, u8 access, bool user_accessible = false, bool cacheable = true);

        /**
         * @param name 
         * @param access 
         * @param user_accessible 
         * @param cacheable 
         * @return OwnPtr<Region> 
         */
        OwnPtr<Region> allocate_kernel_region_with_vmobject(const Range&, VMObject&, const StringView& name, u8 access, bool user_accessible = false, bool cacheable = true);

        /**
         * @param name 
         * @param access 
         * @param cacheable 
         * @return OwnPtr<Region> 
         */
        OwnPtr<Region> allocate_user_accessible_kernel_region(size_t, const StringView& name, u8 access, bool cacheable = true);

        /**
         * @return unsigned 
         */
        unsigned user_physical_pages() const 
        { 
            return m_user_physical_pages; 
        }

        /**
         * @return unsigned 
         */
        unsigned user_physical_pages_used() const 
        { 
            return m_user_physical_pages_used; 
        }

        /**
         * @return unsigned 
         */
        unsigned super_physical_pages() const 
        { 
            return m_super_physical_pages; 
        }

        /**
         * @return unsigned 
         */
        unsigned super_physical_pages_used() const 
        { 
            return m_super_physical_pages_used; 
        }

        /**
         * @tparam Callback 
         * @param callback 
         */
        template<typename Callback>
        static void for_each_vmobject(Callback callback)
        {
            for (auto& vmobject : MM.m_vmobjects) {
                if (callback(vmobject) == IterDecision::Break)
                    break;
            }
        }

        /**
         * @tparam T 
         * @tparam Callback 
         * @param callback 
         */
        template<typename T, typename Callback>
        static void for_each_vmobject_of_type(Callback callback)
        {
            for (auto& vmobject : MM.m_vmobjects) {
                if (!is<T>(vmobject))
                    continue;
                if (callback(static_cast<T&>(vmobject)) == IterDecision::Break)
                    break;
            }
        }

        /**
         * @return Region* 
         */
        static Region* find_region_from_vaddr(Process&, VirtualAddress);

        /**
         * @return const Region* 
         */
        static const Region* find_region_from_vaddr(const Process&, VirtualAddress);

        void dump_kernel_regions();

        /**
         * @return PhysicalPage& 
         */
        PhysicalPage& shared_zero_page() 
        { 
            return *m_shared_zero_page; 
        }

        /**
         * @return PageDirectory& 
         */
        PageDirectory& kernel_page_directory() 
        { 
            return *m_kernel_page_directory; 
        }

    private:
        MemoryManager();
        ~MemoryManager();

        enum class AccessSpace { Kernel,
            User };
        enum class AccessType { Read,
            Write };

        /**
         * @tparam ccessSpace 
         * @tparam AccessType 
         * @return true 
         * @return false 
         */
        template<AccessSpace, AccessType>
        bool validate_range(const Process&, VirtualAddress, size_t) const;
        
        void register_vmobject(VMObject&);
        void unregister_vmobject(VMObject&);
        void register_region(Region&);
        void unregister_region(Region&);

        void detect_cpu_features();
        void protect_kernel_image();
        void parse_memory_map();

        /**
         * @param page_count 
         */
        static void flush_tlb_local(VirtualAddress, size_t page_count = 1);

        /**
         * @param page_count 
         */
        static void flush_tlb(VirtualAddress, size_t page_count = 1);

        /**
         * @return Region* 
         */
        static Region* user_region_from_vaddr(Process&, VirtualAddress);

        /**
         * @return Region* 
         */
        static Region* kernel_region_from_vaddr(VirtualAddress);

        /**
         * @return Region* 
         */
        static Region* find_region_from_vaddr(VirtualAddress);

        /**
         * @return RefPtr<PhysicalPage> 
         */
        RefPtr<PhysicalPage> find_free_user_physical_page();

        /**
         * @return u8* 
         */
        u8* quickmap_page(PhysicalPage&);

        void unquickmap_page();

        /**
         * @param pdpt_index 
         * @return PageDirectoryEntry* 
         */
        PageDirectoryEntry* quickmap_pd(PageDirectory&, size_t pdpt_index);

        /**
         * @return PageTableEntry* 
         */
        PageTableEntry* quickmap_pt(PhysicalAddress);

        /**
         * @return PageTableEntry* 
         */
        PageTableEntry* pte(PageDirectory&, VirtualAddress);

        /**
         * @return PageTableEntry* 
         */
        PageTableEntry* ensure_pte(PageDirectory&, VirtualAddress);

        void release_pte(PageDirectory&, VirtualAddress, bool);

        RefPtr<PageDirectory> m_kernel_page_directory;
        RefPtr<PhysicalPage> m_low_page_table;

        RefPtr<PhysicalPage> m_shared_zero_page;

        unsigned m_user_physical_pages { 0 };
        unsigned m_user_physical_pages_used { 0 };
        unsigned m_super_physical_pages { 0 };
        unsigned m_super_physical_pages_used { 0 };

        NonnullRefPtrVector<PhysicalRegion> m_user_physical_regions;
        NonnullRefPtrVector<PhysicalRegion> m_super_physical_regions;

        InlineLinkedList<Region> m_user_regions;
        InlineLinkedList<Region> m_kernel_regions;

        InlineLinkedList<VMObject> m_vmobjects;

        RefPtr<PhysicalPage> m_low_pseudo_identity_mapping_pages[4];
    };

    /**
     * @tparam Callback 
     * @param callback 
     */
    template<typename Callback>
    void VMObject::for_each_region(Callback callback)
    {
        ScopedSpinLock lock(s_mm_lock);
        for (auto& region : MM.m_user_regions) {
            if (&region.vmobject() == this)
                callback(region);
        }
        for (auto& region : MM.m_kernel_regions) {
            if (&region.vmobject() == this)
                callback(region);
        }
    }
    
    /**
     * @param vaddr 
     * @return true 
     * @return false 
     */
    inline bool is_user_address(VirtualAddress vaddr)
    {
        return vaddr.get() < 0xc0000000;
    }

    /**
     * @param vaddr 
     * @param size 
     * @return true 
     * @return false 
     */
    inline bool is_user_range(VirtualAddress vaddr, size_t size)
    {
        if (vaddr.offset(size) < vaddr)
            return false;
        return is_user_address(vaddr) && is_user_address(vaddr.offset(size));
    }

    /**
     * @return true 
     * @return false 
     */
    inline bool PhysicalPage::is_shared_zero_page() const
    {
        return this == &MM.shared_zero_page();
    }

} // namespace Kernel