/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/WeakRef.h>

namespace JS {

class WeakRefPrototype final : public Object {
    JS_OBJECT(WeakRefPrototype, Object);

public:
    WeakRefPrototype(GlobalObject&);
    virtual void initialize(GlobalObject&) override;
    virtual ~WeakRefPrototype() override;

private:
    JS_DECLARE_NATIVE_FUNCTION(deref);
};

}
