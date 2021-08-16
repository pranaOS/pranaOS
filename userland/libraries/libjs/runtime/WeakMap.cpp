/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/WeakMap.h>

namespace JS {

WeakMap* WeakMap::create(GlobalObject& global_object)
{
    return global_object.heap().allocate<WeakMap>(global_object, *global_object.weak_map_prototype());
}

WeakMap::WeakMap(Object& prototype)
    : Object(prototype)
    , WeakContainer(heap())
{
}

WeakMap::~WeakMap()
{
}

void WeakMap::remove_swept_cells(Badge<Heap>, Vector<Cell*>& cells)
{
    for (auto* cell : cells)
        m_values.remove(cell);
}

}
