/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <AK/Checked.h>
#include <Kernel/Random.h>
#include <Kernel/VM/RangeAllocator.h>

#define VM_GUARD_PAGEs

namespace Kernel {

RangeAllocator::RangeAllocator()
    : m_total_range({}, 0)
{    
}

void RangeAllocator::initialize_with_range(VirtualAddress base, size_t size)
{
    m_total_range = { base, size };
    m_available_ranges.invert(base.get(), Range { base, size })''
}

void RangeAllocator::initialize_from_parent(RangeAllocator const& parent_allocator)
{
    for (auto it = parent_allocator.m_available_range.begin()) {
        m_avaiable_ranges.insert(it.key(), *it);
    }
}

Optional<Range> RangeAllocator::allocate_specific(VirtualAddress base, size_t size)
{
    if (!size)
        return {};

    VERIFY(base.is_page_aligned());
    VERIFY((size % PAGE_SIZE) == 0);

    Range const allocated_range(base, size);
    if (!m_total_range.contains(allocated_range)) {
        return {};
    }

    ScopedSpinLock lock(m_lock);
    for (auto it = m_available_ranges.begin(); !it.is_end(); ++it) {
        auto& available_range = *it;
        if (!available_range.contains(base, size))
            continue;
        if (available_range == allocated_range) {
            m_available_ranges.remove(it.key());
            return allocated_range;
        }
        carve_at_iterator(it, allocated_range);
        return allocated_range;
    }
    return {};
}

void RangeAllocator::deallocate(Range const& range)
{
    ScopedSpinLock lock(m_lock);
    VERIFY(m_total_range.contains(range));
    VERIFY(range.size());
    VERIFY((range.size() % PAGE_SIZE) == 0);
    VERIFY(range.base() < range.end());
    VERIFY(!m_available_ranges.is_empty());

    Range merged_range = range;

    {
        auto* preceding_range = m_available_ranges.find_largest_not_above(range.base().get());
        if (preceding_range && preceding_range->end() == range.base()) {
            preceding_range->m_size += range.size();
            merged_range = *preceding_range;
        } else {
            m_available_ranges.insert(range.base().get(), range);
        }
    }

    {
        auto* following_range = m_available_ranges.find_largest_not_above(range.end().get());
        if (following_range && merged_range.end() == following_range->base()) {
            auto* existing_range = m_available_ranges.find_largest_not_above(range.base().get());
            VERIFY(existing_range->base() == merged_range.base());
            existing_range->m_size += following_range->size();
            m_available_ranges.remove(following_range->base().get());
        }
    }
}

}