/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/NativeFunction.h>

namespace JS {

class GeneratorFunctionConstructor final : public NativeFunction {
    JS_OBJECT(GeneratorFunctionConstructor, NativeFunction);

public:
    explicit GeneratorFunctionConstructor(GlobalObject&);
    virtual void initialize(GlobalObject&) override;
    virtual ~GeneratorFunctionConstructor() override;

    virtual Value call() override;
    virtual Value construct(FunctionObject& new_target) override;

private:
    bool has_constructor() const override { return true; }
};

}
