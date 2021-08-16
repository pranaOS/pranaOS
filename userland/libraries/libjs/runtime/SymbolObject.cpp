/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/Symbol.h>
#include <libjs/runtime/SymbolObject.h>

namespace JS {

SymbolObject* SymbolObject::create(GlobalObject& global_object, Symbol& primitive_symbol)
{
    return global_object.heap().allocate<SymbolObject>(global_object, primitive_symbol, *global_object.symbol_prototype());
}

SymbolObject::SymbolObject(Symbol& symbol, Object& prototype)
    : Object(prototype)
    , m_symbol(symbol)
{
}

SymbolObject::~SymbolObject()
{
}

void SymbolObject::visit_edges(Cell::Visitor& visitor)
{
    Object::visit_edges(visitor);
    visitor.visit(&m_symbol);
}

}
