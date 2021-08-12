/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <base/OwnPtr.h>
#include <kernel/memory/PhysicalPage.h>
#include <kernel/memory/PhysicalZone.h>

namespace Kernel::Memory {

class PhysicalRegion {
    BASE_MAKE_ETERNAL;
    BASE_MAKE_NONCOPYABLE(PhysicalRegion);
    BASE_MAKE_NONMOVABLE(PhysicalRegion);

public:
    static OwnPtr<PhysicalRegion> try_create(PhysicalAddress lower, PhysicalAddress upper)
    {
        return adopt_own_if_nonnull(new PhysicalRegion { lower, upper });
    }

    ~PhysicalRegion();

    void initialize_zones();

    PhysicalAddress lower() const { return m_lower; }
    PhysicalAddress upper() const { return m_upper; }
    unsigned size() const { return m_pages; }
    bool contains(PhysicalAddress paddr) const { return paddr >= m_lower && paddr < m_upper; }

    OwnPtr<PhysicalRegion> try_take_pages_from_beginning(unsigned);

    RefPtr<PhysicalPage> take_free_page();
    NonnullRefPtrVector<PhysicalPage> take_contiguous_free_pages(size_t count);
    void return_page(PhysicalAddress);

private:
    PhysicalRegion(PhysicalAddress lower, PhysicalAddress upper);

    NonnullOwnPtrVector<PhysicalZone> m_zones;

    PhysicalZone::List m_usable_zones;
    PhysicalZone::List m_full_zones;

    PhysicalAddress m_lower;
    PhysicalAddress m_upper;
    unsigned m_pages { 0 };
};

}
