/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/WeakRef.h>

namespace JS {

WeakRef* WeakRef::create(GlobalObject& global_object, Object* object)
{
    return global_object.heap().allocate<WeakRef>(global_object, object, *global_object.weak_ref_prototype());
}

WeakRef::WeakRef(Object* object, Object& prototype)
    : Object(prototype)
    , WeakContainer(heap())
    , m_value(object)
    , m_last_execution_generation(vm().execution_generation())
{
}

WeakRef::~WeakRef()
{
}

void WeakRef::remove_swept_cells(Badge<Heap>, Vector<Cell*>& cells)
{
    VERIFY(m_value);
    for (auto* cell : cells) {
        if (m_value != cell)
            continue;
        m_value = nullptr;

        WeakContainer::deregister();
        break;
    }
}

void WeakRef::visit_edges(Visitor& visitor)
{
    Object::visit_edges(visitor);

    if (vm().execution_generation() == m_last_execution_generation)
        visitor.visit(m_value);
}

}
