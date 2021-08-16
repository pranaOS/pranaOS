/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/Object.h>

namespace JS::Intl {

class Intl final : public Object {
    JS_OBJECT(Intl, Object);

public:
    explicit Intl(GlobalObject&);
    virtual void initialize(GlobalObject&) override;
    virtual ~Intl() override = default;
};

}
