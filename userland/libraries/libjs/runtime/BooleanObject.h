/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/Object.h>

namespace JS {
class BooleanObject : public Object {
    JS_OBJECT(BooleanObject, Object);

public:
    static BooleanObject* create(GlobalObject&, bool);

    BooleanObject(bool, Object& prototype);
    virtual ~BooleanObject() override;

    virtual Value value_of() const override
    {
        return Value(m_value);
    }

private:
    bool m_value { false };
};
}
