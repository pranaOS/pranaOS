/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/TypeCasts.h>
#include <libjs/runtime/WeakRefPrototype.h>

namespace JS {

WeakRefPrototype::WeakRefPrototype(GlobalObject& global_object)
    : Object(*global_object.object_prototype())
{
}

void WeakRefPrototype::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    Object::initialize(global_object);

    define_native_function(vm.names.deref, deref, 0, Attribute::Writable | Attribute::Configurable);

    define_direct_property(*vm.well_known_symbol_to_string_tag(), js_string(vm, vm.names.WeakRef.as_string()), Attribute::Configurable);
}

WeakRefPrototype::~WeakRefPrototype()
{
}

JS_DEFINE_NATIVE_FUNCTION(WeakRefPrototype::deref)
{
    auto* this_object = vm.this_value(global_object).to_object(global_object);
    if (!this_object)
        return {};
    if (!is<WeakRef>(this_object)) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotA, "WeakRef");
        return {};
    }
    auto& weak_ref = static_cast<WeakRef&>(*this_object);
    weak_ref.update_execution_generation();
    return weak_ref.value() ?: js_undefined();
}

}
