/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/GeneratorFunctionConstructor.h>
#include <libjs/runtime/GeneratorFunctionPrototype.h>
#include <libjs/runtime/GeneratorObjectPrototype.h>

namespace JS {

GeneratorFunctionPrototype::GeneratorFunctionPrototype(GlobalObject& global_object)
    : Object(*global_object.function_prototype())
{
}

void GeneratorFunctionPrototype::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    Object::initialize(global_object);

    define_direct_property(vm.names.prototype, global_object.generator_object_prototype(), Attribute::Configurable);

    define_direct_property(*vm.well_known_symbol_to_string_tag(), js_string(vm, "GeneratorFunction"), Attribute::Configurable);
}

GeneratorFunctionPrototype::~GeneratorFunctionPrototype()
{
}

}
