/**
 * @file physicalpage.h
 * @author Krisna Pranav
 * @brief physical page
 * @version 1.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/arch/i386/cpu.h>
#include <kernel/assertions.h>
#include <kernel/heap/slaballocator.h>
#include <kernel/physical_address.h>
#include <mods/nonnullrefptr.h>

namespace Kernel
{
    class PhysicalPage
    {
        friend class MemoryManager;
        friend class PageDirectory;
        friend class VMObject;

        MAKE_SLAB_ALLOCATED(PhysicalPage);
        MOD_MAKE_NONMOVABLE(PhysicalPage);

    public:

        /**
         * @return PhysicalAddress 
         */
        PhysicalAddress paddr() const
        {
            return m_paddr;
        }

        void ref()
        {
            m_ref_count.fetch_add(1, Mods::memory_order_acq_rel);
        }

        void unref()
        {
            if (m_ref_count.fetch_sub(1, Mods::memory_order_acq_rel) == 1) {
                if (m_may_return_to_freelist)
                    return_to_freelist();
                delete this;
            }
        }

    private:
        /**
         * @param paddr 
         * @param supervisor 
         * @param may_return_to_freelist 
         */
        PhysicalPage(PhysicalAddress paddr, bool supervisor, bool may_return_to_freelist = true);

        ~PhysicalPage() {}

        void return_to_freelist() const;

        Atomic<u32> m_ref_count { 1 };
        bool m_may_return_to_freelist { true };
        bool m_supervisor { false };
        PhysicalAddress m_paddr;

    }; // class PhysicalPage
} // namespace Kernel