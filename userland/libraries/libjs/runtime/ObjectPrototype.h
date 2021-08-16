/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/Object.h>

namespace JS {

class ObjectPrototype final : public Object {
    JS_OBJECT(ObjectPrototype, Object);

public:
    explicit ObjectPrototype(GlobalObject&);
    virtual void initialize(GlobalObject&) override;
    virtual ~ObjectPrototype() override;


    virtual bool internal_set_prototype_of(Object* prototype) override;

    JS_DECLARE_NATIVE_FUNCTION(to_string);

private:
    JS_DECLARE_NATIVE_FUNCTION(has_own_property);
    JS_DECLARE_NATIVE_FUNCTION(to_locale_string);
    JS_DECLARE_NATIVE_FUNCTION(value_of);
    JS_DECLARE_NATIVE_FUNCTION(property_is_enumerable);
    JS_DECLARE_NATIVE_FUNCTION(is_prototype_of);
    JS_DECLARE_NATIVE_FUNCTION(define_getter);
    JS_DECLARE_NATIVE_FUNCTION(define_setter);
    JS_DECLARE_NATIVE_FUNCTION(lookup_getter);
    JS_DECLARE_NATIVE_FUNCTION(lookup_setter);
    JS_DECLARE_NATIVE_FUNCTION(proto_getter);
    JS_DECLARE_NATIVE_FUNCTION(proto_setter);
};

}
