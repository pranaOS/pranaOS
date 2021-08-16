/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/Set.h>

namespace JS {

class SetPrototype final : public Object {
    JS_OBJECT(SetPrototype, Object);

public:
    SetPrototype(GlobalObject&);
    virtual void initialize(GlobalObject&) override;
    virtual ~SetPrototype() override;

private:
    static Set* typed_this(VM&, GlobalObject&);

    JS_DECLARE_NATIVE_FUNCTION(add);
    JS_DECLARE_NATIVE_FUNCTION(clear);
    JS_DECLARE_NATIVE_FUNCTION(delete_);
    JS_DECLARE_NATIVE_FUNCTION(entries);
    JS_DECLARE_NATIVE_FUNCTION(for_each);
    JS_DECLARE_NATIVE_FUNCTION(has);
    JS_DECLARE_NATIVE_FUNCTION(values);

    JS_DECLARE_NATIVE_GETTER(size_getter);
};

}
