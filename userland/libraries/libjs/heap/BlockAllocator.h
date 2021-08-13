/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/Vector.h>
#include <libjs/Forward.h>

namespace JS {

class BlockAllocator {
public:
    BlockAllocator();
    ~BlockAllocator();

    void* allocate_block(char const* name);
    void deallocate_block(void*);

private:
    static constexpr size_t max_cached_blocks = 64;

    Vector<void*, max_cached_blocks> m_blocks;
};

}
