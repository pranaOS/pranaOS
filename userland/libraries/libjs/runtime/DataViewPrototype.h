/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/DataView.h>

namespace JS {

class DataViewPrototype final : public Object {
    JS_OBJECT(DataViewPrototype, Object);

public:
    DataViewPrototype(GlobalObject&);
    virtual void initialize(GlobalObject&) override;
    virtual ~DataViewPrototype() override;

private:
    JS_DECLARE_NATIVE_FUNCTION(get_big_int_64);
    JS_DECLARE_NATIVE_FUNCTION(get_big_uint_64);
    JS_DECLARE_NATIVE_FUNCTION(get_float_32);
    JS_DECLARE_NATIVE_FUNCTION(get_float_64);
    JS_DECLARE_NATIVE_FUNCTION(get_int_8);
    JS_DECLARE_NATIVE_FUNCTION(get_int_16);
    JS_DECLARE_NATIVE_FUNCTION(get_int_32);
    JS_DECLARE_NATIVE_FUNCTION(get_uint_8);
    JS_DECLARE_NATIVE_FUNCTION(get_uint_16);
    JS_DECLARE_NATIVE_FUNCTION(get_uint_32);
    JS_DECLARE_NATIVE_FUNCTION(set_big_int_64);
    JS_DECLARE_NATIVE_FUNCTION(set_big_uint_64);
    JS_DECLARE_NATIVE_FUNCTION(set_float_32);
    JS_DECLARE_NATIVE_FUNCTION(set_float_64);
    JS_DECLARE_NATIVE_FUNCTION(set_int_8);
    JS_DECLARE_NATIVE_FUNCTION(set_int_16);
    JS_DECLARE_NATIVE_FUNCTION(set_int_32);
    JS_DECLARE_NATIVE_FUNCTION(set_uint_8);
    JS_DECLARE_NATIVE_FUNCTION(set_uint_16);
    JS_DECLARE_NATIVE_FUNCTION(set_uint_32);

    JS_DECLARE_NATIVE_GETTER(buffer_getter);
    JS_DECLARE_NATIVE_GETTER(byte_length_getter);
    JS_DECLARE_NATIVE_GETTER(byte_offset_getter);
};

}
