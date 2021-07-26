/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/RedBlackTree.h>
#include <base/Vector.h>
#include <base/WeakPtr.h>
#include <kernel/UnixTypes.h>
#include <kernel/vm/AllocationStrategy.h>
#include <kernel/vm/PageDirectory.h>

namespace Kernel {

class Space {
public:
    static OwnPtr<Space> create(Process&, const Space* parent);
    ~Space();

    PageDirectory& page_directory() { return *m_page_directory; }

    Region* add_region(NonnullOwnPtr<Region>);

    size_t region_count() const { return m_regions.size(); }

    void dump_regions();


private:
    Space(Process&, NonnullRefPtr<PageDirectory>);

    struct RegionLookupCache {
        Optional<Range> range;
        WeakPtr<Region> region;
    };

    bool m_enforces_syscall_regions { false };

};

}