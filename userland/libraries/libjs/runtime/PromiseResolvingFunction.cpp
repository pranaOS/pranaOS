/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/NativeFunction.h>
#include <libjs/runtime/Promise.h>
#include <libjs/runtime/PromiseResolvingFunction.h>

namespace JS {

PromiseResolvingFunction* PromiseResolvingFunction::create(GlobalObject& global_object, Promise& promise, AlreadyResolved& already_resolved, FunctionType function)
{
    return global_object.heap().allocate<PromiseResolvingFunction>(global_object, promise, already_resolved, move(function), *global_object.function_prototype());
}

PromiseResolvingFunction::PromiseResolvingFunction(Promise& promise, AlreadyResolved& already_resolved, FunctionType native_function, Object& prototype)
    : NativeFunction(prototype)
    , m_promise(promise)
    , m_already_resolved(already_resolved)
    , m_native_function(move(native_function))
{
}

void PromiseResolvingFunction::initialize(GlobalObject& global_object)
{
    Base::initialize(global_object);
    define_direct_property(vm().names.length, Value(1), Attribute::Configurable);
}

Value PromiseResolvingFunction::call()
{
    return m_native_function(vm(), global_object(), m_promise, m_already_resolved);
}

void PromiseResolvingFunction::visit_edges(Cell::Visitor& visitor)
{
    Base::visit_edges(visitor);
    visitor.visit(&m_promise);
    visitor.visit(&m_already_resolved);
}

}
