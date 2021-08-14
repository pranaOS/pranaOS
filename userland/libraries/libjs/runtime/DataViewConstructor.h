/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/NativeFunction.h>

namespace JS {

class DataViewConstructor final : public NativeFunction {
    JS_OBJECT(DataViewConstructor, NativeFunction);

public:
    explicit DataViewConstructor(GlobalObject&);
    virtual void initialize(GlobalObject&) override;
    virtual ~DataViewConstructor() override;

    virtual Value call() override;
    virtual Value construct(FunctionObject&) override;

private:
    virtual bool has_constructor() const override { return true; }
};

}
