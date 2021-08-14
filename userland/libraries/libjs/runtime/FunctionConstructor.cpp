/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/AST.h>
#include <libjs/Interpreter.h>
#include <libjs/Lexer.h>
#include <libjs/Parser.h>
#include <libjs/runtime/Error.h>
#include <libjs/runtime/FunctionConstructor.h>
#include <libjs/runtime/FunctionObject.h>
#include <libjs/runtime/GlobalObject.h>

namespace JS {

FunctionConstructor::FunctionConstructor(GlobalObject& global_object)
    : NativeFunction(vm().names.Function.as_string(), *global_object.function_prototype())
{
}

void FunctionConstructor::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    NativeFunction::initialize(global_object);

    define_direct_property(vm.names.prototype, global_object.function_prototype(), 0);

    define_direct_property(vm.names.length, Value(1), Attribute::Configurable);
}

FunctionConstructor::~FunctionConstructor()
{
}

RefPtr<FunctionExpression> FunctionConstructor::create_dynamic_function_node(GlobalObject& global_object, FunctionObject&, FunctionKind kind)
{
    auto& vm = global_object.vm();
    String parameters_source = "";
    String body_source = "";
    if (vm.argument_count() == 1) {
        body_source = vm.argument(0).to_string(global_object);
        if (vm.exception())
            return {};
    }
    if (vm.argument_count() > 1) {
        Vector<String> parameters;
        for (size_t i = 0; i < vm.argument_count() - 1; ++i) {
            parameters.append(vm.argument(i).to_string(global_object));
            if (vm.exception())
                return {};
        }
        StringBuilder parameters_builder;
        parameters_builder.join(',', parameters);
        parameters_source = parameters_builder.build();
        body_source = vm.argument(vm.argument_count() - 1).to_string(global_object);
        if (vm.exception())
            return {};
    }
    auto is_generator = kind == FunctionKind::Generator;
    auto source = String::formatted("function{} anonymous({}\n) {{\n{}\n}}", is_generator ? "*" : "", parameters_source, body_source);
    auto parser = Parser(Lexer(source));
    auto function = parser.parse_function_node<FunctionExpression>();
    if (parser.has_errors()) {
        auto error = parser.errors()[0];
        vm.throw_exception<SyntaxError>(global_object, error.to_string());
        return {};
    }

    return function;
}

Value FunctionConstructor::call()
{
    return construct(*this);
}

Value FunctionConstructor::construct(FunctionObject& new_target)
{
    auto function = create_dynamic_function_node(global_object(), new_target, FunctionKind::Regular);
    if (!function)
        return {};

    OwnPtr<Interpreter> local_interpreter;
    Interpreter* interpreter = vm().interpreter_if_exists();

    if (!interpreter) {
        local_interpreter = Interpreter::create_with_existing_global_object(global_object());
        interpreter = local_interpreter.ptr();
    }

    VM::InterpreterExecutionScope scope(*interpreter);
    return function->execute(*interpreter, global_object());
}

}
