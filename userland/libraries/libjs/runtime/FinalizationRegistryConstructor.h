/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/NativeFunction.h>

namespace JS {

class FinalizationRegistryConstructor final : public NativeFunction {
    JS_OBJECT(FinalizationRegistryConstructor, NativeFunction);

public:
    explicit FinalizationRegistryConstructor(GlobalObject&);
    virtual void initialize(GlobalObject&) override;
    virtual ~FinalizationRegistryConstructor() override;

    virtual Value call() override;
    virtual Value construct(FunctionObject&) override;

private:
    virtual bool has_constructor() const override { return true; }
};

}
