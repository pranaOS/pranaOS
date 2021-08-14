/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/NumberObject.h>

namespace JS {

class NumberPrototype final : public NumberObject {
    JS_OBJECT(NumberPrototype, NumberObject);

public:
    explicit NumberPrototype(GlobalObject&);
    virtual void initialize(GlobalObject&) override;
    virtual ~NumberPrototype() override;

    JS_DECLARE_NATIVE_FUNCTION(to_fixed);
    JS_DECLARE_NATIVE_FUNCTION(to_string);
    JS_DECLARE_NATIVE_FUNCTION(value_of);
};

}
