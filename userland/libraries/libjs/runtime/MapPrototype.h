/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/Map.h>

namespace JS {

class MapPrototype final : public Object {
    JS_OBJECT(MapPrototype, Object);

public:
    MapPrototype(GlobalObject&);
    virtual void initialize(GlobalObject&) override;
    virtual ~MapPrototype() override;

private:
    static Map* typed_this(VM&, GlobalObject&);

    JS_DECLARE_NATIVE_FUNCTION(clear);
    JS_DECLARE_NATIVE_FUNCTION(delete_);
    JS_DECLARE_NATIVE_FUNCTION(entries);
    JS_DECLARE_NATIVE_FUNCTION(for_each);
    JS_DECLARE_NATIVE_FUNCTION(get);
    JS_DECLARE_NATIVE_FUNCTION(has);
    JS_DECLARE_NATIVE_FUNCTION(keys);
    JS_DECLARE_NATIVE_FUNCTION(set);
    JS_DECLARE_NATIVE_FUNCTION(values);

    JS_DECLARE_NATIVE_GETTER(size_getter);
};

}
