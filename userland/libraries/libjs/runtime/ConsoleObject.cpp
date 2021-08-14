/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/Console.h>
#include <libjs/runtime/ConsoleObject.h>
#include <libjs/runtime/GlobalObject.h>

namespace JS {

ConsoleObject::ConsoleObject(GlobalObject& global_object)
    : Object(*global_object.object_prototype())
{
}

void ConsoleObject::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    Object::initialize(global_object);
    u8 attr = Attribute::Writable | Attribute::Enumerable | Attribute::Configurable;
    define_native_function(vm.names.log, log, 0, attr);
    define_native_function(vm.names.debug, debug, 0, attr);
    define_native_function(vm.names.info, info, 0, attr);
    define_native_function(vm.names.warn, warn, 0, attr);
    define_native_function(vm.names.error, error, 0, attr);
    define_native_function(vm.names.trace, trace, 0, attr);
    define_native_function(vm.names.count, count, 0, attr);
    define_native_function(vm.names.countReset, count_reset, 0, attr);
    define_native_function(vm.names.clear, clear, 0, attr);
    define_native_function(vm.names.assert, assert_, 0, attr);
}

ConsoleObject::~ConsoleObject()
{
}

JS_DEFINE_NATIVE_FUNCTION(ConsoleObject::log)
{
    return global_object.console().log();
}

JS_DEFINE_NATIVE_FUNCTION(ConsoleObject::debug)
{
    return global_object.console().debug();
}

JS_DEFINE_NATIVE_FUNCTION(ConsoleObject::info)
{
    return global_object.console().info();
}

JS_DEFINE_NATIVE_FUNCTION(ConsoleObject::warn)
{
    return global_object.console().warn();
}

JS_DEFINE_NATIVE_FUNCTION(ConsoleObject::error)
{
    return global_object.console().error();
}

JS_DEFINE_NATIVE_FUNCTION(ConsoleObject::trace)
{
    return global_object.console().trace();
}

JS_DEFINE_NATIVE_FUNCTION(ConsoleObject::count)
{
    return global_object.console().count();
}

JS_DEFINE_NATIVE_FUNCTION(ConsoleObject::count_reset)
{
    return global_object.console().count_reset();
}

JS_DEFINE_NATIVE_FUNCTION(ConsoleObject::clear)
{
    return global_object.console().clear();
}

JS_DEFINE_NATIVE_FUNCTION(ConsoleObject::assert_)
{
    return global_object.console().assert_();
}

}
