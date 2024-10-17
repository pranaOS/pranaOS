/**
 * @file physicalpage.h
 * @author Krisna Pranav
 * @brief Physical Page
 * @version 6.0
 * @date 2024-10-17
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <kernel/physicaladdress.h>
#include <mods/nonnullrefptr.h>

namespace Kernel::Memory
{

    enum class MayReturnToFreeList : bool
    {
        No,
        Yes
    }; // enum class MayReturnToFreeList : bool

    class PhysicalPage
    {
        MOD_MAKE_NONCOPYABLE(PhysicalPage);
        MOD_MAKE_NONMOVABLE(PhysicalPage);

        friend class MemoryManager;

    public:
        /**
         * @return PhysicalAddress 
         */
        PhysicalAddress paddr() const;

        void ref()
        {
            m_ref_count.fetch_add(1, Mods::memory_order_acq_rel);
        }

        void unref()
        {
            if(m_ref_count.fetch_sub(1, Mods::memory_order_acq_rel) == 1)
                free_this();
        }

        /**
         * @param may_return_to_freelist 
         * @return NonnullRefPtr<PhysicalPage> 
         */
        static NonnullRefPtr<PhysicalPage> create(PhysicalAddress, MayReturnToFreeList may_return_to_freelist = MayReturnToFreeList::Yes);

        /**
         * @return u32 
         */
        u32 ref_count() const
        {
            return m_ref_count.load(Mods::memory_order_consume);
        }

        bool is_shared_zero_page() const;
        bool is_lazy_committed_page() const;

    private:
        /**
         * @brief Construct a new Physical Page object
         * 
         * @param may_return_to_freelist 
         */
        explicit PhysicalPage(MayReturnToFreeList may_return_to_freelist);

        /**
         * @brief Destroy the Physical Page object
         * 
         */
        ~PhysicalPage() = default;

        void free_this();

        Atomic<u32> m_ref_count{1};
        MayReturnToFreeList m_may_return_to_freelist{MayReturnToFreeList::Yes};
    }; // class PhysicalPage

    struct PhysicalPageEntry
    {
        union
        {
            struct
            {
                PhysicalPage physical_page;
            } allocated;

            struct
            {
                i16 next_index;
                i16 prev_index;
            } freelist;
        };
    }; // struct PhysicalPageEntry

} // namespace Kernel::Memory