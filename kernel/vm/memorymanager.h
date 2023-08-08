/**
 * @file memorymanager.h
 * @author Krisna Pranav
 * @brief memory_manager
 * @version 1.0
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
    #define PAGE_ROUND_UP(x)

    /**
     * @tparam T 
     * @param physical 
     * @return T* 
     */
    template<typename T>
    inline T* low_physical_to_virtual(T* physical)
    {
        return (T*) (((u8*)physical) + 0xc00000000);
    }

    class KBuffer;
    class SynthFSInode;

    #define MM Kernel::MemoryManager::the()

    struct MemoryManagerData 
    {
        SpinLock<u8> m_quickmap_in_use;
        u32 m_quickmap_prev_flags;
        PhysicalAddress m_last_quickmap_pd;
    };

    extern RecursiveSpinLock s_mm_lock;

    class MemoryManager
    {
        MOD_MAKE_ETERNAL
        friend class PageDirectory;
        friend class PhysicalPage;
        friend class PhysicalRange;
        friend class Region;
        friend class VMObject;

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
        PageFaultResponse handle_page_fauilt(const PageFault&);

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
        };

        template<typename Callback>
        static void for_each_vmobject(Callback callback)
        {
            for (auto& vmobject : MM.m_vmobjects) {
                if (callback(vmobject) == IterDecision::Break) {
                    break;
                }
            }
        }
    }

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
        
        return is_user_address(vaddr) & is_user_address(vaddr.offset(size));
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