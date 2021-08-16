/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/NativeFunction.h>

namespace JS {

class ArrayConstructor final : public NativeFunction {
    JS_OBJECT(ArrayConstructor, NativeFunction);

public:
    explicit ArrayConstructor(GlobalObject&);
    virtual void initialize(GlobalObject&) override;
    virtual ~ArrayConstructor() override;

    virtual Value call() override;
    virtual Value construct(FunctionObject& new_target) override;

private:
    virtual bool has_constructor() const override { return true; }

    JS_DECLARE_NATIVE_FUNCTION(from);
    JS_DECLARE_NATIVE_FUNCTION(is_array);
    JS_DECLARE_NATIVE_FUNCTION(of);

    JS_DECLARE_NATIVE_GETTER(symbol_species_getter);
};

}
