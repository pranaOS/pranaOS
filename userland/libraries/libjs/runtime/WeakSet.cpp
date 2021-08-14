/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/WeakSet.h>

namespace JS {

WeakSet* WeakSet::create(GlobalObject& global_object)
{
    return global_object.heap().allocate<WeakSet>(global_object, *global_object.weak_set_prototype());
}

WeakSet::WeakSet(Object& prototype)
    : Object(prototype)
    , WeakContainer(heap())
{
}

WeakSet::~WeakSet()
{
}

void WeakSet::remove_swept_cells(Badge<Heap>, Vector<Cell*>& cells)
{
    for (auto* cell : cells)
        m_values.remove(cell);
}

}
