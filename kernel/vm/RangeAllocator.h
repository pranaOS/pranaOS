/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/RedBlackTree.h>
#include <base/Traits.h>
#include <kernel/SpinLock.h>
#include <kernel/vm/Range.h>

namespace Kernel {

class RangeAllocator {
public:
    RangeAllocator();
    ~RangeAllocator() = default;

    void initialize_with_range(VirtualAddress, size_t);
    void initialize_from_parent(RangeAllocator const&);

    void dump() const;

private:
    void crave_at_iteartor(auto&, Range const&);

    mutable SpinLock<u8> m_lock;

};

}

namespace Base {
template<>
}