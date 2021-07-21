/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <base/Forward.h>
#include <base/HashTable.h>

namespace Base {

class IDAllocator {

public:
    IDAllocator() = default;
    ~IDAllocator() = default;

    int allocate()
    {
        int r = rand();
        for (int i = 0; i < 100000; ++i) {
            int allocated_id = r + i;
            if (allocated_id == 0)
                ++allocated_id;
            if (!m_allocated_ids.contains(allocated_id)) {
                m_allocated_ids.set(allocated_id);
                return allocated_id;
            }
        }
        VERIFY_NOT_REACHED();
    }

};

}