/**
 * @file physicalregion.h
 * @author Krisna Pranav
 * @brief Physical Region
 * @version 6.0
 * @date 2024-10-20
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <kernel/memory/physicalpage.h>
#include <kernel/memory/physicalzone.h>
#include <mods/ownptr.h>

namespace Kernel::Memory
{
    class PhysicalRegion
    {
        MOD_MAKE_NONCOPYABLE(PhysicalRegion);
        MOD_MAKE_NONMOVABLE(PhysicalRegion);

    public:
        /**
         * @param lower 
         * @param upper 
         * @return OwnPtr<PhysicalRegion> 
         */
        static OwnPtr<PhysicalRegion> try_create(PhysicalAddress lower, PhysicalAddress upper)
        {
            return adopt_own_if_nonnull(new PhysicalRegion{lower, upper});
        }

        /**
         * @brief Destroy the Physical Region object
         * 
         */
        ~PhysicalRegion();

        void initialize_zones();

        /**
         * @return PhysicalAddress 
         */
        PhysicalAddress lower() const
        {
            return m_lower;
        }

        /**
         * @return PhysicalAddress 
         */
        PhysicalAddress upper() const
        {
            return m_upper;
        }

        /**
         * @return unsigned 
         */
        unsigned size() const
        {
            return m_pages;
        }

        /**
         * @param paddr 
         * @return true 
         * @return false 
         */
        bool contains(PhysicalAddress paddr) const
        {
            return paddr >= m_lower && paddr < m_upper;
        }

        /**
         * @return OwnPtr<PhysicalRegion> 
         */
        OwnPtr<PhysicalRegion> try_take_pages_from_beginning(unsigned);

        /**
         * @return RefPtr<PhysicalPage> 
         */
        RefPtr<PhysicalPage> take_free_page();

        /**
         * @param count 
         * @return NonnullRefPtrVector<PhysicalPage> 
         */
        NonnullRefPtrVector<PhysicalPage> take_contiguous_free_pages(size_t count);

        void return_page(PhysicalAddress);

    private:
        /**
         * @brief Construct a new Physical Region object
         * 
         * @param lower 
         * @param upper 
         */
        PhysicalRegion(PhysicalAddress lower, PhysicalAddress upper);

        static constexpr size_t large_zone_size = 16 * MiB;
        static constexpr size_t small_zone_size = 1 * MiB;

        NonnullOwnPtrVector<PhysicalZone> m_zones;

        size_t m_large_zones{0};

        PhysicalZone::List m_usable_zones;
        PhysicalZone::List m_full_zones;

        PhysicalAddress m_lower;
        PhysicalAddress m_upper;
        unsigned m_pages{0};
    }; // class PhysicalRegion
} // namespace Kernel::Memory
