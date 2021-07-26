/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/PerformanceManager.h>
#include <kernel/Process.h>
#include <kernel/SpinLock.h>
#include <kernel/vm/AnonymousVMObject.h>
#include <kernel/vm/InodeVMObject.h>
#include <kernel/vm/MemoryManager.h>
#include <kernel/vm/Space.h>

namespace Kernel {

OwnPtr<Space> Space::create(Process& process, const Space* parent)
{
    auto page_directory = PageDirectory::create_for_userspace(parent ? &parent->page_directory().range_allocator() : nullptr);
    if (!page_directory)
        return {};
    auto space = adopt_own_if_nonnull(new (nothrow) Space(process, page_directory.release_nonnull()));
    if (!space)
        return {};
    space->page_directory().set_space({}, *space);
    return space;
}

Space::Space(Process& process, NonnullRefPtr<PageDirectory> page_directory)
    : m_process(&process)
    , m_page_directory(move(page_directory))
{
}

Space::~Space()
{
}

void Space::deallocate_region(Region& region)
{
    take_region(region);
}

NonnullOwnPtr<Region> Space::take_region(Region& region)
{
    ScopedSpinLock lock(m_lock);

    if (m_region_lookup_cache.region.unsafe_ptr() == &region)
        m_region_lookup_cache.region = nullptr;

    auto found_region = m_regions.unsafe_remove(region.vaddr().get());
    VERIFY(found_region.ptr() == &region);
    return found_region;
}

Region* Space::find_region_from_range(const Range& range)
{
    ScopedSpinLock lock(m_lock);
    if (m_region_lookup_cache.range.has_value() && m_region_lookup_cache.range.value() == range && m_region_lookup_cache.region)
        return m_region_lookup_cache.region.unsafe_ptr();

    auto found_region = m_regions.find(range.base().get());
    if (!found_region)
        return nullptr;
    auto& region = *found_region;
    size_t size = page_round_up(range.size());
    if (region->size() != size)
        return nullptr;
    m_region_lookup_cache.range = range;
    m_region_lookup_cache.region = *region;
    return region;
}

Region* Space::find_region_containing(const Range& range)
{
    ScopedSpinLock lock(m_lock);
    auto candidate = m_regions.find_largest_not_above(range.base().get());
    if (!candidate)
        return nullptr;
    return (*candidate)->range().contains(range) ? candidate->ptr() : nullptr;
}

Region* Space::add_region(NonnullOwnPtr<Region> region)
{
    auto* ptr = region.ptr();
    ScopedSpinLock lock(m_lock);
    auto success = m_regions.try_insert(region->vaddr().get(), move(region));
    return success ? ptr : nullptr;
}

KResultOr<Vector<Region*, 2>> Space::try_split_region_around_range(const Region& source_region, const Range& desired_range)
{
    Range old_region_range = source_region.range();
    auto remaining_ranges_after_unmap = old_region_range.carve(desired_range);

    VERIFY(!remaining_ranges_after_unmap.is_empty());
    auto try_make_replacement_region = [&](const Range& new_range) -> KResultOr<Region*> {
        VERIFY(old_region_range.contains(new_range));
        size_t new_range_offset_in_vmobject = source_region.offset_in_vmobject() + (new_range.base().get() - old_region_range.base().get());
        return try_allocate_split_region(source_region, new_range, new_range_offset_in_vmobject);
    };
    Vector<Region*, 2> new_regions;
    for (auto& new_range : remaining_ranges_after_unmap) {
        auto new_region_or_error = try_make_replacement_region(new_range);
        if (new_region_or_error.is_error())
            return new_region_or_error.error();
        new_regions.unchecked_append(new_region_or_error.value());
    }
    return new_regions;
}


}