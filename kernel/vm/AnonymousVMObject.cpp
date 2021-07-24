/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include <kernel/arch/x86/SmapDisabler.h>
#include <kernel/Debug.h>
#include <kernel/Process.h>
#include <Kernel/vm/AnonymousVMObject.h>
#include <kernel/vm/MemoryManage.h>
#include <kernel/vm/PhysicalPage.h>

namespace Kernel {

RefPtr<VMObject> AnonymousVMObject::try_clone()
{
    ScopedSpinLock lock(m_lock);

    size_t need_cow_pages = 0;

    for_each_nonvolatile_range([&](VolatilePageRange const& nonvolatile_range) {
        need_cow_pages += nonvolatile_range.count;
    });

    dbgln_if(COMMIT_DEBUG, "Cloning {:p}, need {} committed cow pages", this, need_cow_pages);

    if (!MM.commit_user_physical_pages(need_cow_pages))
        return {};

    m_shared_committed_cow_pages = try_create<CommittedCowPages>(need_cow_pages);

    if (!m_shared_committed_cow_pages) {
        MM.uncommit_user_physical_pages(need_cow_pages);
        return {};
    }

    ensure_or_reset_cow_map();

    return adopt_ref_if_nonnull(new (nothrow) AnonymousVMObject(*this));
}

RefPtr<AnonymousVMObject> AnonymousVMObject::try_create_with_size(size_t size, AllocationStrategy commit)
{
    if (commit == AllocationStrategy::Reserve || commit == AllocationStrategy::AllocateNow) {
        if (!MM.commit_user_physical_pages(ceil_div(size, static_cast<size_t>(PAGE_SIZE))))
            return {};
    }
    return adopt_ref_if_nonnull(new (nothrow) AnonymousVMObject(size, commit));
}

RefPtr<AnonymousVMObject> AnonymousVMObject::try_create_with_physical_pages(Span<NonnullRefPtr<PhysicalPage>> physical_pages)
{
    return adopt_ref_if_nonnull(new (nothrow) AnonymousVMObject(physical_pages));
}

RefPtr<AnonymousVMObject> AnonymousVMObject::try_create_for_physical_range(PhysicalAddress paddr, size_t size)
{
    if (paddr.offset(size) < paddr) {
        dbgln("Shenanigans! try_create_for_physical_range({}, {}) would wrap around", paddr, size);
        return nullptr;
    }
    return adopt_ref_if_nonnull(new (nothrow) AnonymousVMObject(paddr, size));
}

AnonymousVMObject::AnonymousVMObject(size_t size, AllocationStrategy strategy)
    : VMObject(size)
    , m_volatile_ranges_cache({ 0, page_count() })
    , m_unused_committed_pages(strategy == AllocationStrategy::Reserve ? page_count() : 0)
{
    if (strategy == AllocationStrategy::AllocateNow) {
        for (size_t i = 0; i < page_count(); ++i)
            physical_pages()[i] = MM.allocate_committed_user_physical_page(MemoryManager::ShouldZeroFill::Yes);
    } else {
        auto& initial_page = (strategy == AllocationStrategy::Reserve) ? MM.lazy_committed_page() : MM.shared_zero_page();
        for (size_t i = 0; i < page_count(); ++i)
            physical_pages()[i] = initial_page;
    }
}

AnonymousVMObject::AnonymousVMObject(PhysicalAddress paddr, size_t size)
    : VMObject(size)
    , m_volatile_ranges_cache({ 0, page_count() })
{
    VERIFY(paddr.page_base() == paddr);
    for (size_t i = 0; i < page_count(); ++i)
        physical_pages()[i] = PhysicalPage::create(paddr.offset(i * PAGE_SIZE), MayReturnToFreeList::No);
}

AnonymousVMObject::AnonymousVMObject(Span<NonnullRefPtr<PhysicalPage>> physical_pages)
    : VMObject(physical_pages.size() * PAGE_SIZE)
    , m_volatile_ranges_cache({ 0, page_count() })
{
    for (size_t i = 0; i < physical_pages.size(); ++i) {
        m_physical_pages[i] = physical_pages[i];
    }
}

AnonymousVMObject::AnonymousVMObject(AnonymousVMObject const& other)
    : VMObject(other)
    , m_volatile_ranges_cache({ 0, page_count() }) 
    , m_volatile_ranges_cache_dirty(true)          
    , m_purgeable_ranges()                         
    , m_unused_committed_pages(other.m_unused_committed_pages)
    , m_cow_map()                                                      
    , m_shared_committed_cow_pages(other.m_shared_committed_cow_pages) 
{
    VERIFY(other.m_lock.is_locked());
    m_lock.initialize();

    ensure_or_reset_cow_map();

    if (m_unused_committed_pages > 0) {
        for (size_t i = 0; i < page_count(); i++) {
            auto& phys_page = m_physical_pages[i];
            if (phys_page && phys_page->is_lazy_committed_page()) {
                phys_page = MM.shared_zero_page();
                if (--m_unused_committed_pages == 0)
                    break;
            }
        }
        VERIFY(m_unused_committed_pages == 0);
    }
}

AnonymousVMObject::~AnonymousVMObject()
{
    if (m_unused_committed_pages > 0)
        MM.uncommit_user_physical_pages(m_unused_committed_pages);
}

int AnonymousVMObject::purge()
{
    int purged_page_count = 0;
    ScopedSpinLock lock(m_lock);
    for_each_volatile_range([&](auto const& range) {
        int purged_in_range = 0;
        auto range_end = range.base + range.count;
        for (size_t i = range.base; i < range_end; i++) {
            auto& phys_page = m_physical_pages[i];
            if (phys_page && !phys_page->is_shared_zero_page()) {
                VERIFY(!phys_page->is_lazy_committed_page());
                ++purged_in_range;
            }
            phys_page = MM.shared_zero_page();
        }

        if (purged_in_range > 0) {
            purged_page_count += purged_in_range;
            set_was_purged(range);
            for_each_region([&](auto& region) {
                if (auto owner = region.get_owner()) {
                    dmesgln("Purged {} pages from region {} owned by {} at {} - {}",
                        purged_in_range,
                        region.name(),
                        *owner,
                        region.vaddr_from_page_index(range.base),
                        region.vaddr_from_page_index(range.base + range.count));
                } else {
                    dmesgln("Purged {} pages from region {} (no ownership) at {} - {}",
                        purged_in_range,
                        region.name(),
                        region.vaddr_from_page_index(range.base),
                        region.vaddr_from_page_index(range.base + range.count));
                }
                region.remap_vmobject_page_range(range.base, range.count);
            });
        }
    });
    return purged_page_count;
}

void AnonymousVMObject::set_was_purged(VolatilePageRange const& range)
{
    VERIFY(m_lock.is_locked());
    for (auto* purgeable_ranges : m_purgeable_ranges)
        purgeable_ranges->set_was_purged(range);
}

void AnonymousVMObject::register_purgeable_page_ranges(PurgeablePageRanges& purgeable_page_ranges)
{
    ScopedSpinLock lock(m_lock);
    purgeable_page_ranges.set_vmobject(this);
    VERIFY(!m_purgeable_ranges.contains_slow(&purgeable_page_ranges));
    m_purgeable_ranges.append(&purgeable_page_ranges);
}

void AnonymousVMObject::unregister_purgeable_page_ranges(PurgeablePageRanges& purgeable_page_ranges)
{
    ScopedSpinLock lock(m_lock);
    for (size_t i = 0; i < m_purgeable_ranges.size(); i++) {
        if (m_purgeable_ranges[i] != &purgeable_page_ranges)
            continue;
        purgeable_page_ranges.set_vmobject(nullptr);
        m_purgeable_ranges.remove(i);
        return;
    }
    VERIFY_NOT_REACHED();
}

bool AnonymousVMObject::is_any_volatile() const
{
    ScopedSpinLock lock(m_lock);
    for (auto& volatile_ranges : m_purgeable_ranges) {
        ScopedSpinLock lock(volatile_ranges->m_volatile_ranges_lock);
        if (!volatile_ranges->is_empty())
            return true;
    }
    return false;
}


}