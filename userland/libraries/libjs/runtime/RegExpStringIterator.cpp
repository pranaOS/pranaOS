/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/RegExpStringIterator.h>

namespace JS {

RegExpStringIterator* RegExpStringIterator::create(GlobalObject& global_object, Object& regexp_object, Utf16String string, bool global, bool unicode)
{
    return global_object.heap().allocate<RegExpStringIterator>(global_object, *global_object.regexp_string_iterator_prototype(), regexp_object, move(string), global, unicode);
}

RegExpStringIterator::RegExpStringIterator(Object& prototype, Object& regexp_object, Utf16String string, bool global, bool unicode)
    : Object(prototype)
    , m_regexp_object(regexp_object)
    , m_string(move(string))
    , m_global(global)
    , m_unicode(unicode)
{
}

void RegExpStringIterator::visit_edges(Cell::Visitor& visitor)
{
    Object::visit_edges(visitor);
    visitor.visit(&m_regexp_object);
}

}
