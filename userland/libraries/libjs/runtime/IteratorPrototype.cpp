/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/Function.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/IteratorPrototype.h>

namespace JS {

IteratorPrototype::IteratorPrototype(GlobalObject& global_object)
    : Object(*global_object.object_prototype())
{
}

void IteratorPrototype::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    Object::initialize(global_object);
    u8 attr = Attribute::Writable | Attribute::Enumerable;
    define_native_function(*vm.well_known_symbol_iterator(), symbol_iterator, 0, attr);
}

IteratorPrototype::~IteratorPrototype()
{
}

JS_DEFINE_NATIVE_FUNCTION(IteratorPrototype::symbol_iterator)
{
    auto* this_object = vm.this_value(global_object).to_object(global_object);
    if (!this_object)
        return {};
    return this_object;
}

}
