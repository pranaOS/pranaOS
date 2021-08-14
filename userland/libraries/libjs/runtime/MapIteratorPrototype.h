/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/Object.h>

namespace JS {

class MapIteratorPrototype final : public Object {
    JS_OBJECT(MapIteratorPrototype, Object)

public:
    MapIteratorPrototype(GlobalObject&);
    virtual void initialize(GlobalObject&) override;
    virtual ~MapIteratorPrototype() override;

private:
    JS_DECLARE_NATIVE_FUNCTION(next);
};

}
