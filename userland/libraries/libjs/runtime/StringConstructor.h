/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/NativeFunction.h>

namespace JS {

class StringConstructor final : public NativeFunction {
    JS_OBJECT(StringConstructor, NativeFunction);

public:
    explicit StringConstructor(GlobalObject&);
    virtual void initialize(GlobalObject&) override;
    virtual ~StringConstructor() override;

    virtual Value call() override;
    virtual Value construct(FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

    JS_DECLARE_NATIVE_FUNCTION(raw);
    JS_DECLARE_NATIVE_FUNCTION(from_char_code);
    JS_DECLARE_NATIVE_FUNCTION(from_code_point);
};

}
