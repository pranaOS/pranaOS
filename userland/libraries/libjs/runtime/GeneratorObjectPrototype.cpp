/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/bytecode/Interpreter.h>
#include <libjs/runtime/GeneratorObject.h>
#include <libjs/runtime/GeneratorObjectPrototype.h>

namespace JS {

static GeneratorObject* typed_this(VM& vm, GlobalObject& global_object)
{
    auto* this_object = vm.this_value(global_object).to_object(global_object);
    if (!this_object)
        return {};
    if (!is<GeneratorObject>(this_object)) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotA, "Generator");
        return nullptr;
    }
    return static_cast<GeneratorObject*>(this_object);
}

GeneratorObjectPrototype::GeneratorObjectPrototype(GlobalObject& global_object)
    : Object(*global_object.iterator_prototype())
{
}

void GeneratorObjectPrototype::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    Object::initialize(global_object);
    u8 attr = Attribute::Writable | Attribute::Configurable;
    define_native_function(vm.names.next, next, 1, attr);
    define_native_function(vm.names.return_, return_, 1, attr);
    define_native_function(vm.names.throw_, throw_, 1, attr);

    define_direct_property(*vm.well_known_symbol_to_string_tag(), js_string(vm, "Generator"), Attribute::Configurable);
}

GeneratorObjectPrototype::~GeneratorObjectPrototype()
{
}

JS_DEFINE_NATIVE_FUNCTION(GeneratorObjectPrototype::next)
{
    auto generator_object = typed_this(vm, global_object);
    if (!generator_object)
        return {};
    return generator_object->next_impl(vm, global_object, {});
}

JS_DEFINE_NATIVE_FUNCTION(GeneratorObjectPrototype::return_)
{
    auto generator_object = typed_this(vm, global_object);
    if (!generator_object)
        return {};
    generator_object->set_done();
    return generator_object->next_impl(vm, global_object, {});
}

JS_DEFINE_NATIVE_FUNCTION(GeneratorObjectPrototype::throw_)
{
    auto generator_object = typed_this(vm, global_object);
    if (!generator_object)
        return {};
    return generator_object->next_impl(vm, global_object, vm.argument(0));
}

}
