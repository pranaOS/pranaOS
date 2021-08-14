/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/NumberObject.h>

namespace JS {

NumberObject* NumberObject::create(GlobalObject& global_object, double value)
{
    return global_object.heap().allocate<NumberObject>(global_object, value, *global_object.number_prototype());
}

NumberObject::NumberObject(double value, Object& prototype)
    : Object(prototype)
    , m_value(value)
{
}

NumberObject::~NumberObject()
{
}

}
