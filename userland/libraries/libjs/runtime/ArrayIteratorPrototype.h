/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/Object.h>

namespace JS {

class ArrayIteratorPrototype final : public Object {
    JS_OBJECT(ArrayIteratorPrototype, Object)

public:
    ArrayIteratorPrototype(GlobalObject&);
    virtual void initialize(GlobalObject&) override;
    virtual ~ArrayIteratorPrototype() override;

private:
    JS_DECLARE_NATIVE_FUNCTION(next);
};

}
