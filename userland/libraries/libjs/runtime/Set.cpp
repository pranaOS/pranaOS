/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/Set.h>

namespace JS {

Set* Set::create(GlobalObject& global_object)
{
    return global_object.heap().allocate<Set>(global_object, *global_object.set_prototype());
}

Set::Set(Object& prototype)
    : Object(prototype)
{
}

Set::~Set()
{
}

void Set::visit_edges(Cell::Visitor& visitor)
{
    Object::visit_edges(visitor);
    for (auto& value : m_values)
        visitor.visit(value);
}

}
