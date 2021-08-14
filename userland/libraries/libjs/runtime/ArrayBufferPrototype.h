/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/Object.h>

namespace JS {

class ArrayBufferPrototype final : public Object {
    JS_OBJECT(ArrayBufferPrototype, Object);

public:
    explicit ArrayBufferPrototype(GlobalObject&);
    virtual void initialize(GlobalObject&) override;
    virtual ~ArrayBufferPrototype() override;

private:
    JS_DECLARE_NATIVE_FUNCTION(slice);
    JS_DECLARE_NATIVE_GETTER(byte_length_getter);
};

}
