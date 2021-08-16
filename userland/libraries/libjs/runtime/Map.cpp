/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/Map.h>

namespace JS {

Map* Map::create(GlobalObject& global_object)
{
    return global_object.heap().allocate<Map>(global_object, *global_object.map_prototype());
}

Map::Map(Object& prototype)
    : Object(prototype)
{
}

Map::~Map()
{
}

void Map::visit_edges(Cell::Visitor& visitor)
{
    Object::visit_edges(visitor);
    for (auto& value : m_entries) {
        visitor.visit(value.key);
        visitor.visit(value.value);
    }
}

}
