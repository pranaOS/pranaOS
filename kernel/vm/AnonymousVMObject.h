/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <kernel/PhysicalAddress.h>
#include <kernel/vm/AllocationStrategy.h>
#include <kernel/vm/MemoryManager.h>
#include <kernel/vm/PageFaultResponse.h>
#include <kernel/vm/PurgeablePageRanges.h>
#include <kernel/vm/VMObject.h>

namespace Kernel {

class AnonymousVMObject final : public VMObject {
    friend class PurgeablePageRanges;
    
public:
    virtual ~AnonymousVMObject() override;

    static RefPtr<AnonymousVMObject> try_create_with_size(size_t, AllocationStrategy);
    static RefPtr<AnonymousVMObject> try_create_for_physical_range(PhysicalAddress paddr, size_t size);
    static RefPtr<AnonymousVMObject> try_create_with_physical_pages(Span<NonnullRefPtr<PhysicalPage>>);
    virtual RefPtr<VMObject> try_clone() override;

    [[nodiscard]] NonnullRefPtr<PhysicalPage> allocate_committed_page(Badge<Region>, size_t);
    PageFaultResponse handle_cow_fault(size_t, VirtualAddress);
    size_t cow_pages() const;
    bool should_cow(size_t page_index, bool) const;
    void set_should_cow(size_t page_index, bool);

    void register_purgeable_page_ranges(PurgeablePageRanges&);
    void unregister_purgeable_page_ranges(PurgeablePageRanges&);

    int purge();

    bool is_any_volatile() const;
}
    
}