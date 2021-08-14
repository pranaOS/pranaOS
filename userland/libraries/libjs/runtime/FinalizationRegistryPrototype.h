/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/FinalizationRegistry.h>

namespace JS {

class FinalizationRegistryPrototype final : public Object {
    JS_OBJECT(FinalizationRegistryPrototype, Object);

public:
    FinalizationRegistryPrototype(GlobalObject&);
    virtual void initialize(GlobalObject&) override;
    virtual ~FinalizationRegistryPrototype() override;

private:
    static FinalizationRegistry* typed_this(VM&, GlobalObject&);

    JS_DECLARE_NATIVE_FUNCTION(cleanup_some);
    JS_DECLARE_NATIVE_FUNCTION(register_);
    JS_DECLARE_NATIVE_FUNCTION(unregister);
};

}
