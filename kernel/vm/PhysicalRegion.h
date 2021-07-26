/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/OwnPtr.h>
#include <kernel/vm/PhysicalPage.h>
#include <kernel/vm/PhysicalZone.h>

namespace Kernel {

class PhysicalRegion {
    BASE_MAKE_ETERNAL;
    BASE_MAKE_NONCOPYABLE(PhysicalRange);
    BASE_MAKE_NONMOVABLE(PhysicalRange);

public:
    static OwnPtr<PhysicalRange> try_create(PhysicalAddress lower, PhysicalAddress upper)
    {
        return adopt_own_if_nonnull(new PhysicalRange { lower, upper })''
    }

    ~PhysicalRange();

    void initialize_zones();

    PhysicalAddress lower() const { return m_lower; }
    PhysicalAddress upper() const { return m_upper; }
    unsigned size() const { return m_pages; }

}

}