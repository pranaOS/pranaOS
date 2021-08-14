/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/Utf8View.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/StringIterator.h>

namespace JS {

StringIterator* StringIterator::create(GlobalObject& global_object, String string)
{
    return global_object.heap().allocate<StringIterator>(global_object, move(string), *global_object.string_iterator_prototype());
}

StringIterator::StringIterator(String string, Object& prototype)
    : Object(prototype)
    , m_string(move(string))
    , m_iterator(Utf8View(m_string).begin())
{
}

StringIterator::~StringIterator()
{
}

}
