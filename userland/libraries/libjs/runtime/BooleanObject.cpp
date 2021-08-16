/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/BooleanObject.h>
#include <libjs/runtime/GlobalObject.h>

namespace JS {

BooleanObject* BooleanObject::create(GlobalObject& global_object, bool value)
{
    return global_object.heap().allocate<BooleanObject>(global_object, value, *global_object.boolean_prototype());
}

BooleanObject::BooleanObject(bool value, Object& prototype)
    : Object(prototype)
    , m_value(value)
{
}

BooleanObject::~BooleanObject()
{
}

}
