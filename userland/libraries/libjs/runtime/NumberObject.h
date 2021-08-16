/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/Object.h>

namespace JS {

class NumberObject : public Object {
    JS_OBJECT(NumberObject, Object);

public:
    static NumberObject* create(GlobalObject&, double);

    NumberObject(double, Object& prototype);
    virtual ~NumberObject() override;

    virtual Value value_of() const override { return Value(m_value); }

    double number() const { return m_value; }

private:
    double m_value { 0 };
};

}
