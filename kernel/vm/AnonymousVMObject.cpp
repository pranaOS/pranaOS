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


}