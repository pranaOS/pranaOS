/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libcore/Property.h>

namespace Core {

Property::Property(String name, Function<JsonValue()> getter, Function<bool(const JsonValue&)> setter)
    : m_name(move(name))
    , m_getter(move(getter))
    , m_setter(move(setter))
{
}

Property::~Property()
{
}

}
