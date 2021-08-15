/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/FlyString.h>

namespace Web {

class Attribute {
public:
    Attribute(const FlyString& name, const String& value)
        : m_name(name)
        , m_value(value)
    {
    }

    const FlyString& name() const { return m_name; }
    const String& value() const { return m_value; }

    void set_value(const String& value) { m_value = value; }

private:
    FlyString m_name;
    String m_value;
};

}
