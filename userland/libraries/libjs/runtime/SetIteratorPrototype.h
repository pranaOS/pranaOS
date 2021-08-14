/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/Object.h>

namespace JS {

class SetIteratorPrototype final : public Object {
    JS_OBJECT(SetIteratorPrototype, Object)

public:
    SetIteratorPrototype(GlobalObject&);
    virtual void initialize(GlobalObject&) override;
    virtual ~SetIteratorPrototype() override;

private:
    JS_DECLARE_NATIVE_FUNCTION(next);
};

}
