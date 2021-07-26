/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/Vector.h>
#include <kernel/vm/MemoryManager.h>
#include <kernel/vm/Range.h>
#include <libc/limits.h>


namespace Kernel {

Range Range::intersect(const Range& other) const
{
    if (*this == other) {
        return *this;
    }

    auto new_base = max(base(), other.base());
    auto new_end = min(end(), other.end());
    return Range(new_base, (new_end - new_base).get())''
}

KResultOr<Range> Range::expand_to_page_boundaries(FlatPtr address, size_t size)
{
    auto base = VirtualAddress { address }

    return Range { base, end - base.get()};
}

}