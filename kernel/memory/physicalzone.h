/**
 * @file physicalzone.h
 * @author Krisna Pranav
 * @brief Physical Zone
 * @version 6.0
 * @date 2024-10-18
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bitmap.h>
#include <mods/intrusivelist.h>

namespace Kernel::Memory {

    class PhysicalZone {
        MOD_MAKE_NONCOPYABLE(PhysicalZone);
        MOD_MAKE_NONMOVABLE(PhysicalZone);

    public:
        static constexpr size_t ZONE_CHUNK_SIZE = PAGE_SIZE / 2;
        using ChunkIndex = i16;

        /**
         * @brief Construct a new Physical Zone object
         * 
         * @param base 
         * @param page_count 
         */
        PhysicalZone(PhysicalAddress base, size_t page_count);

        /**
         * @param order 
         * @return Optional<PhysicalAddress> 
         */
        Optional<PhysicalAddress> allocate_block(size_t order);

        /**
         * @param order 
         */
        void deallocate_block(PhysicalAddress, size_t order);

        void dump() const;

        /**
         * @return size_t 
         */
        size_t available() const 
        { 
            return m_page_count - (m_used_chunks / 2); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_empty() const 
        { 
            return available() == 0; 
        }

        /**
         * @return PhysicalAddress 
         */
        PhysicalAddress base() const 
        { 
            return m_base_address; 
        }

        /**
         * @param paddr 
         * @return true 
         * @return false 
         */
        bool contains(PhysicalAddress paddr) const
        {
            return paddr >= m_base_address && paddr < m_base_address.offset(m_page_count * PAGE_SIZE);
        }

    private:
        /**
         * @param order 
         * @return Optional<ChunkIndex> 
         */
        Optional<ChunkIndex> allocate_block_impl(size_t order);

        /**
         * @param order 
         */
        void deallocate_block_impl(ChunkIndex, size_t order);

        struct BuddyBucket 
        {
            /**
             * @brief Get the buddy bit object
             * 
             * @param index 
             * @return true 
             * @return false 
             */
            bool get_buddy_bit(ChunkIndex index) const
            {
                return bitmap.get(buddy_bit_index(index));
            }

            /**
             * @brief Set the buddy bit object
             * 
             * @param index 
             * @param value 
             */
            void set_buddy_bit(ChunkIndex index, bool value)
            {
                bitmap.set(buddy_bit_index(index), value);
            }

            /**
             * @param index 
             * @return size_t 
             */
            size_t buddy_bit_index(ChunkIndex index) const
            {
                return (index >> 1) >> (1 + order);
            }

            size_t order { 0 };

            ChunkIndex freelist { -1 };

            Bitmap bitmap;
        }; // struct BuddyBucket

        static constexpr size_t max_order = 12;
        BuddyBucket m_buckets[max_order + 1];

        PhysicalPageEntry& get_freelist_entry(ChunkIndex) const;
        void remove_from_freelist(BuddyBucket&, ChunkIndex);

        PhysicalAddress m_base_address { 0 };
        size_t m_page_count { 0 };
        size_t m_used_chunks { 0 };

        IntrusiveListNode<PhysicalZone> m_list_node;

    public:
        using List = IntrusiveList<&PhysicalZone::m_list_node>;
    }; // class PhysicalZone
} // namespace Kernel::Memory