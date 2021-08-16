/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/Object.h>

namespace JS {

class StringIteratorPrototype final : public Object {
    JS_OBJECT(StringIteratorPrototype, Object)

public:
    StringIteratorPrototype(GlobalObject&);
    virtual void initialize(GlobalObject&) override;
    virtual ~StringIteratorPrototype() override;

private:
    JS_DECLARE_NATIVE_FUNCTION(next);
};

}
