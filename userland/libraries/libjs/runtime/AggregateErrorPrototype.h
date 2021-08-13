/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/Object.h>

namespace JS {

class AggregateErrorPrototype final : public Object {
    JS_OBJECT(AggregateErrorPrototype, Object);

public:
    explicit AggregateErrorPrototype(GlobalObject&);
    virtual void initialize(GlobalObject&) override;
    virtual ~AggregateErrorPrototype() override = default;
};

}
