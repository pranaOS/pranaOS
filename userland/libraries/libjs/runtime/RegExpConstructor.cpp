/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/Error.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/RegExpConstructor.h>
#include <libjs/runtime/RegExpObject.h>

namespace JS {

RegExpConstructor::RegExpConstructor(GlobalObject& global_object)
    : NativeFunction(vm().names.RegExp.as_string(), *global_object.function_prototype())
{
}

void RegExpConstructor::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    NativeFunction::initialize(global_object);

    define_direct_property(vm.names.prototype, global_object.regexp_prototype(), 0);

    define_native_accessor(*vm.well_known_symbol_species(), symbol_species_getter, {}, Attribute::Configurable);

    define_direct_property(vm.names.length, Value(2), Attribute::Configurable);
}

RegExpConstructor::~RegExpConstructor()
{
}

Value RegExpConstructor::call()
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();

    auto pattern = vm.argument(0);
    auto flags = vm.argument(1);

    bool pattern_is_regexp = pattern.is_regexp(global_object);
    if (vm.exception())
        return {};

    if (pattern_is_regexp && flags.is_undefined()) {
        auto pattern_constructor = pattern.as_object().get(vm.names.constructor);
        if (vm.exception())
            return {};

        if (same_value(this, pattern_constructor))
            return pattern;
    }

    return construct(*this);
}

Value RegExpConstructor::construct(FunctionObject&)
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();

    auto pattern = vm.argument(0);
    auto flags = vm.argument(1);

    bool pattern_is_regexp = pattern.is_regexp(global_object);
    if (vm.exception())
        return {};

    Value pattern_value;
    Value flags_value;

    if (pattern.is_object() && is<RegExpObject>(pattern.as_object())) {
        auto& regexp_pattern = static_cast<RegExpObject&>(pattern.as_object());
        pattern_value = js_string(vm, regexp_pattern.pattern());

        if (flags.is_undefined())
            flags_value = js_string(vm, regexp_pattern.flags());
        else
            flags_value = flags;
    } else if (pattern_is_regexp) {
        pattern_value = pattern.as_object().get(vm.names.source);
        if (vm.exception())
            return {};

        if (flags.is_undefined()) {
            flags_value = pattern.as_object().get(vm.names.flags);
            if (vm.exception())
                return {};
        } else {
            flags_value = flags;
        }
    } else {
        pattern_value = pattern;
        flags_value = flags;
    }

    return regexp_create(global_object, pattern_value, flags_value);
}

JS_DEFINE_NATIVE_GETTER(RegExpConstructor::symbol_species_getter)
{
    return vm.this_value(global_object);
}

}
