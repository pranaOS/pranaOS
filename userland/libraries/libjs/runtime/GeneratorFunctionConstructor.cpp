/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/Debug.h>
#include <base/Optional.h>
#include <libjs/bytecodee/Interpreter.h>
#include <libjs/Lexer.h>
#include <libjs/Parser.h>
#include <libjs/runtime/FunctionConstructor.h>
#include <libjs/runtime/GeneratorFunctionConstructor.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/OrdinaryFunctionObject.h>

namespace JS {

GeneratorFunctionConstructor::GeneratorFunctionConstructor(GlobalObject& global_object)
    : NativeFunction(*static_cast<Object*>(global_object.function_constructor()))
{
}

void GeneratorFunctionConstructor::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    NativeFunction::initialize(global_object);

    define_direct_property(vm.names.length, Value(1), Attribute::Configurable);

    define_direct_property(vm.names.prototype, global_object.generator_function_prototype(), 0);
}

GeneratorFunctionConstructor::~GeneratorFunctionConstructor()
{
}

Value GeneratorFunctionConstructor::call()
{
    return construct(*this);
}

Value GeneratorFunctionConstructor::construct(FunctionObject& new_target)
{
    auto function = FunctionConstructor::create_dynamic_function_node(global_object(), new_target, FunctionKind::Generator);
    if (!function)
        return {};

    auto* bytecode_interpreter = Bytecode::Interpreter::current();
    VERIFY(bytecode_interpreter);

    auto executable = Bytecode::Generator::generate(function->body(), true);
    auto& passes = JS::Bytecode::Interpreter::optimization_pipeline();
    passes.perform(executable);
    if constexpr (JS_BYTECODE_DEBUG) {
        dbgln("Optimisation passes took {}us", passes.elapsed());
        dbgln("Compiled Bytecode::Block for function '{}':", function->name());
        for (auto& block : executable.basic_blocks)
            block.dump(executable);
    }

    return OrdinaryFunctionObject::create(global_object(), function->name(), function->body(), function->parameters(), function->function_length(), vm().lexical_environment(), FunctionKind::Generator, function->is_strict_mode(), false);
}

}
