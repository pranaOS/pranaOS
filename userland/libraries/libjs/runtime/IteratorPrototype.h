/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/Object.h>

namespace JS {

class IteratorPrototype : public Object {
    JS_OBJECT(IteratorPrototype, Object)

public:
    IteratorPrototype(GlobalObject&);
    virtual void initialize(GlobalObject&) override;
    virtual ~IteratorPrototype() override;

private:
    JS_DECLARE_NATIVE_FUNCTION(symbol_iterator);
};

}
