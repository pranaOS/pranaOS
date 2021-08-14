/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/FunctionObject.h>
#include <libjs/runtime/GlobalObject.h>

namespace JS {

class GeneratorFunctionPrototype final : public Object {
    JS_OBJECT(GeneratorFunctionPrototype, Object);

public:
    explicit GeneratorFunctionPrototype(GlobalObject&);
    virtual void initialize(GlobalObject&) override;
    virtual ~GeneratorFunctionPrototype() override;
};

}
