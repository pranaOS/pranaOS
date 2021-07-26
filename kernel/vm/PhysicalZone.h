/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Bitmap.h>
#include <base/IntrusiveList.h>

namespace Kernel {

class PhysicalZone {
    BASE_MAKE_ETERNAL;
    BASE_MAKE_NONCOPYABLE(PhysicalZone);
    BASE_MAKE_NONMOVABLE(PhysicalZone);

public:
    static constexpr size_t ZONE_CHUNK_SIZE = PAGE_SIZE / 2;
    using ChunkIndex = i16;

    PhysicalZone(PhysicalAddress base, size_t page_count);

    Optional<PhysicalAddress> allocate_block(size_t order);
    void deallocate_block(PhysicalAddress, size_t order);

    void dump() const;
    size_t available() const { return m_page_count - (m_used_chunks / 2); }

    bool is_empty() const { return !available(); }

    PhysicalAddress base() const { return m_base_address; }
    bool contains(PhysicalAddress paddr) const
    {
        return paddr >= m_base_address && paddr < m_base_address.offset(m_page_count * PAGE_SIZE);
    }

private:
    Optional<ChunkIndex> allocate_block_impl(size_t order);
    void deallocate_block_impl(ChunkIndex, size_t order);

    struct BuddyBucket {
        bool get_buddy_bit(ChunkIndex index) const
        {
            return bitmap.get(buddy_bit_index(index));
        }

        void set_buddy_bit(ChunkIndex index, bool value)
        {
            bitmap.set(buddy_bit_index(index), value);
        }

        size_t buddy_bit_index(ChunkIndex index) const
        {
            return (index >> 1) >> (1 + order);
        }

        size_t order { 0 };

        ChunkIndex freelist { -1 };

        Bitmap bitmap;
    };

    static constexpr size_t max_order = 12;
    BuddyBucket m_buckets[max_order + 1];

    PhysicalPageEntry& get_freelist_entry(ChunkIndex) const;
    void remove_from_freelist(BuddyBucket&, ChunkIndex);

    PhysicalAddress m_base_address { 0 };
    size_t m_page_count { 0 };
    size_t m_used_chunks { 0 };

    IntrusiveListNode<PhysicalZone> m_list_node;

public:
    using List = IntrusiveList<PhysicalZone, RawPtr<PhysicalZone>, &PhysicalZone::m_list_node>;
};

}