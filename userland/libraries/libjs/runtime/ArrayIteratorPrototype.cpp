/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/AbstractOperations.h>
#include <libjs/runtime/Array.h>
#include <libjs/runtime/ArrayIterator.h>
#include <libjs/runtime/ArrayIteratorPrototype.h>
#include <libjs/runtime/Error.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/IteratorOperations.h>
#include <libjs/runtime/TypedArray.h>

namespace JS {

ArrayIteratorPrototype::ArrayIteratorPrototype(GlobalObject& global_object)
    : Object(*global_object.iterator_prototype())
{
}

void ArrayIteratorPrototype::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    Object::initialize(global_object);

    define_native_function(vm.names.next, next, 0, Attribute::Configurable | Attribute::Writable);

    define_direct_property(*vm.well_known_symbol_to_string_tag(), js_string(global_object.heap(), "Array Iterator"), Attribute::Configurable);
}

ArrayIteratorPrototype::~ArrayIteratorPrototype()
{
}

JS_DEFINE_NATIVE_FUNCTION(ArrayIteratorPrototype::next)
{
    auto this_value = vm.this_value(global_object);
    if (!this_value.is_object() || !is<ArrayIterator>(this_value.as_object())) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAn, "Array Iterator");
        return {};
    }
    auto& this_object = this_value.as_object();
    auto& iterator = static_cast<ArrayIterator&>(this_object);
    auto target_array = iterator.array();
    if (target_array.is_undefined())
        return create_iterator_result_object(global_object, js_undefined(), true);
    VERIFY(target_array.is_object());
    auto& array = target_array.as_object();

    auto index = iterator.index();
    auto iteration_kind = iterator.iteration_kind();

    size_t length;

    if (array.is_typed_array()) {
        auto& typed_array = static_cast<TypedArrayBase&>(array);

        if (typed_array.viewed_array_buffer()->is_detached()) {
            vm.throw_exception<TypeError>(global_object, ErrorType::DetachedArrayBuffer);
            return {};
        }

        length = typed_array.array_length();
    } else {
        length = length_of_array_like(global_object, array);
        if (vm.exception())
            return {};
    }

    if (index >= length) {
        iterator.m_array = js_undefined();
        return create_iterator_result_object(global_object, js_undefined(), true);
    }

    iterator.m_index++;
    if (iteration_kind == Object::PropertyKind::Key)
        return create_iterator_result_object(global_object, Value(static_cast<i32>(index)), false);

    auto value = array.get(index);
    if (vm.exception())
        return {};
    if (iteration_kind == Object::PropertyKind::Value)
        return create_iterator_result_object(global_object, value, false);

    return create_iterator_result_object(global_object, Array::create_from(global_object, { Value(static_cast<i32>(index)), value }), false);
}

}
