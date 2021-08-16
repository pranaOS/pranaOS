/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/BooleanObject.h>

namespace JS {

class BooleanPrototype final : public BooleanObject {
    JS_OBJECT(BooleanPrototype, BooleanObject);

public:
    explicit BooleanPrototype(GlobalObject&);
    virtual void initialize(GlobalObject&) override;
    virtual ~BooleanPrototype() override;

private:
    JS_DECLARE_NATIVE_FUNCTION(to_string);
    JS_DECLARE_NATIVE_FUNCTION(value_of);
};

}
