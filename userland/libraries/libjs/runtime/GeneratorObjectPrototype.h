/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/GlobalObject.h>

namespace JS {

class GeneratorObjectPrototype final : public Object {
    JS_OBJECT(GeneratorObjectPrototype, Object);

public:
    explicit GeneratorObjectPrototype(GlobalObject&);
    virtual void initialize(GlobalObject&) override;
    virtual ~GeneratorObjectPrototype() override;

private:
    JS_DECLARE_NATIVE_FUNCTION(next);
    JS_DECLARE_NATIVE_FUNCTION(return_);
    JS_DECLARE_NATIVE_FUNCTION(throw_);
};

}
