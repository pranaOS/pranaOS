/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <base/Function.h>
#include <base/HashTable.h>
#include <base/ScopeGuard.h>
#include <base/StringBuilder.h>
#include <libjs/runtime/AbstractOperations.h>
#include <libjs/runtime/Array.h>
#include <libjs/runtime/ArrayConstructor.h>
#include <libjs/runtime/ArrayIterator.h>
#include <libjs/runtime/ArrayPrototype.h>
#include <libjs/runtime/Error.h>
#include <libjs/runtime/FunctionObject.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/ObjectPrototype.h>
#include <libjs/runtime/Value.h>

namespace JS {

static HashTable<Object*> s_array_join_seen_objects;

ArrayPrototype::ArrayPrototype(GlobalObject& global_object)
    : Array(*global_object.object_prototype())
{
}

void ArrayPrototype::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    Array::initialize(global_object);
    u8 attr = Attribute::Writable | Attribute::Configurable;

    define_native_function(vm.names.filter, filter, 1, attr);
    define_native_function(vm.names.forEach, for_each, 1, attr);
    define_native_function(vm.names.map, map, 1, attr);
    define_native_function(vm.names.pop, pop, 0, attr);
    define_native_function(vm.names.push, push, 1, attr);
    define_native_function(vm.names.shift, shift, 0, attr);
    define_native_function(vm.names.toString, to_string, 0, attr);
    define_native_function(vm.names.toLocaleString, to_locale_string, 0, attr);
    define_native_function(vm.names.unshift, unshift, 1, attr);
    define_native_function(vm.names.join, join, 1, attr);
    define_native_function(vm.names.concat, concat, 1, attr);
    define_native_function(vm.names.slice, slice, 2, attr);
    define_native_function(vm.names.indexOf, index_of, 1, attr);
    define_native_function(vm.names.reduce, reduce, 1, attr);
    define_native_function(vm.names.reduceRight, reduce_right, 1, attr);
    define_native_function(vm.names.reverse, reverse, 0, attr);
    define_native_function(vm.names.sort, sort, 1, attr);
    define_native_function(vm.names.lastIndexOf, last_index_of, 1, attr);
    define_native_function(vm.names.includes, includes, 1, attr);
    define_native_function(vm.names.find, find, 1, attr);
    define_native_function(vm.names.findIndex, find_index, 1, attr);
    define_native_function(vm.names.findLast, find_last, 1, attr);
    define_native_function(vm.names.findLastIndex, find_last_index, 1, attr);
    define_native_function(vm.names.some, some, 1, attr);
    define_native_function(vm.names.every, every, 1, attr);
    define_native_function(vm.names.splice, splice, 2, attr);
    define_native_function(vm.names.fill, fill, 1, attr);
    define_native_function(vm.names.values, values, 0, attr);
    define_native_function(vm.names.flat, flat, 0, attr);
    define_native_function(vm.names.flatMap, flat_map, 1, attr);
    define_native_function(vm.names.at, at, 1, attr);
    define_native_function(vm.names.keys, keys, 0, attr);
    define_native_function(vm.names.entries, entries, 0, attr);
    define_native_function(vm.names.copyWithin, copy_within, 2, attr);

    define_direct_property(*vm.well_known_symbol_iterator(), get(vm.names.values), attr);

    auto* unscopable_list = Object::create(global_object, nullptr);
    unscopable_list->create_data_property_or_throw(vm.names.copyWithin, Value(true));
    unscopable_list->create_data_property_or_throw(vm.names.entries, Value(true));
    unscopable_list->create_data_property_or_throw(vm.names.fill, Value(true));
    unscopable_list->create_data_property_or_throw(vm.names.find, Value(true));
    unscopable_list->create_data_property_or_throw(vm.names.findIndex, Value(true));
    unscopable_list->create_data_property_or_throw(vm.names.findLast, Value(true));
    unscopable_list->create_data_property_or_throw(vm.names.findLastIndex, Value(true));
    unscopable_list->create_data_property_or_throw(vm.names.flat, Value(true));
    unscopable_list->create_data_property_or_throw(vm.names.flatMap, Value(true));
    unscopable_list->create_data_property_or_throw(vm.names.includes, Value(true));
    unscopable_list->create_data_property_or_throw(vm.names.keys, Value(true));
    unscopable_list->create_data_property_or_throw(vm.names.values, Value(true));

    define_direct_property(*vm.well_known_symbol_unscopables(), unscopable_list, Attribute::Configurable);
}

ArrayPrototype::~ArrayPrototype()
{
}

static Object* array_species_create(GlobalObject& global_object, Object& original_array, size_t length)
{
    auto& vm = global_object.vm();

    auto is_array = Value(&original_array).is_array(global_object);
    if (vm.exception())
        return {};

    if (!is_array) {
        auto array = Array::create(global_object, length);
        if (vm.exception())
            return {};
        return array;
    }

    auto constructor = original_array.get(vm.names.constructor);
    if (vm.exception())
        return {};
    if (constructor.is_constructor()) {
        auto& constructor_function = constructor.as_function();
        auto* constructor_realm = get_function_realm(global_object, constructor_function);
        if (vm.exception())
            return {};
        if (constructor_realm != &global_object) {
            if (&constructor_function == constructor_realm->array_constructor())
                constructor = js_undefined();
        }
    }

    if (constructor.is_object()) {
        constructor = constructor.as_object().get(*vm.well_known_symbol_species());
        if (vm.exception())
            return {};
        if (constructor.is_null())
            constructor = js_undefined();
    }

    if (constructor.is_undefined()) {
        auto array = Array::create(global_object, length);
        if (vm.exception())
            return {};
        return array;
    }

    if (!constructor.is_constructor()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAConstructor, constructor.to_string_without_side_effects());
        return {};
    }

    MarkedValueList arguments(vm.heap());
    arguments.append(Value(length));
    auto result = vm.construct(constructor.as_function(), constructor.as_function(), move(arguments));
    if (vm.exception())
        return {};
    return &result.as_object();
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::filter)
{
    auto callback_function = vm.argument(0);
    auto this_arg = vm.argument(1);

    auto* object = vm.this_value(global_object).to_object(global_object);
    if (vm.exception())
        return {};

    auto length = length_of_array_like(global_object, *object);
    if (vm.exception())
        return {};

    if (!callback_function.is_function()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAFunction, callback_function.to_string_without_side_effects());
        return {};
    }

    auto* array = array_species_create(global_object, *object, 0);
    if (vm.exception())
        return {};

    size_t k = 0;

    size_t to = 0;

    for (; k < length; ++k) {
        auto property_name = PropertyName { k };

        auto k_present = object->has_property(property_name);
        if (vm.exception())
            return {};

        if (k_present) {
            auto k_value = object->get(k);
            if (vm.exception())
                return {};

            auto selected = vm.call(callback_function.as_function(), this_arg, k_value, Value(k), object);
            if (vm.exception())
                return {};

            if (selected.to_boolean()) {
                array->create_data_property_or_throw(to, k_value);

                ++to;
            }
        }

    }

    return array;
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::for_each)
{
    auto callback_function = vm.argument(0);
    auto this_arg = vm.argument(1);

    auto object = vm.this_value(global_object).to_object(global_object);
    if (vm.exception())
        return {};

    auto length = length_of_array_like(global_object, *object);
    if (vm.exception())
        return {};

    if (!callback_function.is_function()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAFunction, callback_function.to_string_without_side_effects());
        return {};
    }

    for (size_t k = 0; k < length; ++k) {
        auto property_name = PropertyName { k };

        auto k_present = object->has_property(property_name);
        if (vm.exception())
            return {};

        if (k_present) {
            auto k_value = object->get(property_name);
            if (vm.exception())
                return {};

            (void)vm.call(callback_function.as_function(), this_arg, k_value, Value(k), object);
            if (vm.exception())
                return {};
        }

    }

    return js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::map)
{
    auto callback_function = vm.argument(0);
    auto this_arg = vm.argument(1);

    auto* object = vm.this_value(global_object).to_object(global_object);
    if (vm.exception())
        return {};

    auto length = length_of_array_like(global_object, *object);
    if (vm.exception())
        return {};

    if (!callback_function.is_function()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAFunction, callback_function.to_string_without_side_effects());
        return {};
    }

    auto* array = array_species_create(global_object, *object, length);
    if (vm.exception())
        return {};

    for (size_t k = 0; k < length; ++k) {
        auto property_name = PropertyName { k };

        auto k_present = object->has_property(property_name);
        if (vm.exception())
            return {};

        if (k_present) {
            auto k_value = object->get(property_name);
            if (vm.exception())
                return {};

            auto mapped_value = vm.call(callback_function.as_function(), this_arg, k_value, Value(k), object);
            if (vm.exception())
                return {};

            array->create_data_property_or_throw(property_name, mapped_value);
            if (vm.exception())
                return {};
        }

    }

    return array;
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::push)
{
    auto* this_object = vm.this_value(global_object).to_object(global_object);
    if (!this_object)
        return {};
    auto length = length_of_array_like(global_object, *this_object);
    if (vm.exception())
        return {};
    auto argument_count = vm.argument_count();
    auto new_length = length + argument_count;
    if (new_length > MAX_ARRAY_LIKE_INDEX) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ArrayMaxSize);
        return {};
    }
    for (size_t i = 0; i < argument_count; ++i) {
        this_object->set(length + i, vm.argument(i), Object::ShouldThrowExceptions::Yes);
        if (vm.exception())
            return {};
    }
    auto new_length_value = Value(new_length);
    this_object->set(vm.names.length, new_length_value, Object::ShouldThrowExceptions::Yes);
    if (vm.exception())
        return {};
    return new_length_value;
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::unshift)
{
    auto* this_object = vm.this_value(global_object).to_object(global_object);
    if (!this_object)
        return {};
    auto length = length_of_array_like(global_object, *this_object);
    if (vm.exception())
        return {};
    auto arg_count = vm.argument_count();
    size_t new_length = length + arg_count;
    if (arg_count > 0) {
        if (new_length > MAX_ARRAY_LIKE_INDEX) {
            vm.throw_exception<TypeError>(global_object, ErrorType::ArrayMaxSize);
            return {};
        }

        for (size_t k = length; k > 0; --k) {
            auto from = k - 1;
            auto to = k + arg_count - 1;

            bool from_present = this_object->has_property(from);
            if (vm.exception())
                return {};
            if (from_present) {
                auto from_value = this_object->get(from);
                if (vm.exception())
                    return {};
                this_object->set(to, from_value, Object::ShouldThrowExceptions::Yes);
                if (vm.exception())
                    return {};
            } else {
                this_object->delete_property_or_throw(to);
                if (vm.exception())
                    return {};
            }
        }

        for (size_t j = 0; j < arg_count; j++) {
            this_object->set(j, vm.argument(j), Object::ShouldThrowExceptions::Yes);
            if (vm.exception())
                return {};
        }
    }

    this_object->set(vm.names.length, Value(new_length), Object::ShouldThrowExceptions::Yes);
    if (vm.exception())
        return {};
    return Value(new_length);
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::pop)
{
    auto* this_object = vm.this_value(global_object).to_object(global_object);
    if (!this_object)
        return {};
    auto length = length_of_array_like(global_object, *this_object);
    if (vm.exception())
        return {};
    if (length == 0) {
        this_object->set(vm.names.length, Value(0), Object::ShouldThrowExceptions::Yes);
        return js_undefined();
    }
    auto index = length - 1;
    auto element = this_object->get(index);
    if (vm.exception())
        return {};
    this_object->delete_property_or_throw(index);
    if (vm.exception())
        return {};
    this_object->set(vm.names.length, Value(index), Object::ShouldThrowExceptions::Yes);
    if (vm.exception())
        return {};
    return element;
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::shift)
{
    auto* this_object = vm.this_value(global_object).to_object(global_object);
    if (!this_object)
        return {};
    auto length = length_of_array_like(global_object, *this_object);
    if (vm.exception())
        return {};
    if (length == 0) {
        this_object->set(vm.names.length, Value(0), Object::ShouldThrowExceptions::Yes);
        if (vm.exception())
            return {};
        return js_undefined();
    }
    auto first = this_object->get(0);
    if (vm.exception())
        return {};

    for (size_t k = 1; k < length; ++k) {
        size_t from = k;
        size_t to = k - 1;
        bool from_present = this_object->has_property(from);
        if (vm.exception())
            return {};
        if (from_present) {
            auto from_value = this_object->get(from);
            if (vm.exception())
                return {};
            this_object->set(to, from_value, Object::ShouldThrowExceptions::Yes);
            if (vm.exception())
                return {};
        } else {
            this_object->delete_property_or_throw(to);
            if (vm.exception())
                return {};
        }
    }

    this_object->delete_property_or_throw(length - 1);
    if (vm.exception())
        return {};

    this_object->set(vm.names.length, Value(length - 1), Object::ShouldThrowExceptions::Yes);
    if (vm.exception())
        return {};
    return first;
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::to_string)
{
    auto* this_object = vm.this_value(global_object).to_object(global_object);
    if (!this_object)
        return {};
    auto join_function = this_object->get(vm.names.join);
    if (vm.exception())
        return {};
    if (!join_function.is_function())
        return ObjectPrototype::to_string(vm, global_object);
    return vm.call(join_function.as_function(), this_object);
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::to_locale_string)
{
    auto* this_object = vm.this_value(global_object).to_object(global_object);
    if (!this_object)
        return {};

    if (s_array_join_seen_objects.contains(this_object))
        return js_string(vm, "");
    s_array_join_seen_objects.set(this_object);
    ArmedScopeGuard unsee_object_guard = [&] {
        s_array_join_seen_objects.remove(this_object);
    };

    auto length = length_of_array_like(global_object, *this_object);
    if (vm.exception())
        return {};

    String separator = ","; // NOTE: This is implementation-specific.
    StringBuilder builder;
    for (size_t i = 0; i < length; ++i) {
        if (i > 0)
            builder.append(separator);
        auto value = this_object->get(i);
        if (vm.exception())
            return {};
        if (value.is_nullish())
            continue;
        auto locale_string_result = value.invoke(global_object, vm.names.toLocaleString);
        if (vm.exception())
            return {};
        auto string = locale_string_result.to_string(global_object);
        if (vm.exception())
            return {};
        builder.append(string);
    }
    return js_string(vm, builder.to_string());
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::join)
{
    auto* this_object = vm.this_value(global_object).to_object(global_object);
    if (!this_object)
        return {};

    if (s_array_join_seen_objects.contains(this_object))
        return js_string(vm, "");
    s_array_join_seen_objects.set(this_object);
    ArmedScopeGuard unsee_object_guard = [&] {
        s_array_join_seen_objects.remove(this_object);
    };

    auto length = length_of_array_like(global_object, *this_object);
    if (vm.exception())
        return {};
    String separator = ",";
    if (!vm.argument(0).is_undefined()) {
        separator = vm.argument(0).to_string(global_object);
        if (vm.exception())
            return {};
    }
    StringBuilder builder;
    for (size_t i = 0; i < length; ++i) {
        if (i > 0)
            builder.append(separator);
        auto value = this_object->get(i);
        if (vm.exception())
            return {};
        if (value.is_nullish())
            continue;
        auto string = value.to_string(global_object);
        if (vm.exception())
            return {};
        builder.append(string);
    }

    return js_string(vm, builder.to_string());
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::concat)
{
    auto* this_object = vm.this_value(global_object).to_object(global_object);
    if (!this_object)
        return {};

    auto* new_array = array_species_create(global_object, *this_object, 0);
    if (vm.exception())
        return {};

    size_t n = 0;

    auto is_concat_spreadable = [&vm, &global_object](Value const& val) {
        if (!val.is_object()) {
            return false;
        }
        auto* object = val.to_object(global_object);
        if (vm.exception())
            return false;

        auto spreadable = object->get(*vm.well_known_symbol_is_concat_spreadable());
        if (vm.exception())
            return false;

        if (!spreadable.is_undefined())
            return spreadable.to_boolean();

        return val.is_array(global_object);
    };

    auto append_to_new_array = [&vm, &is_concat_spreadable, &new_array, &global_object, &n](Value arg) {
        auto spreadable = is_concat_spreadable(arg);
        if (vm.exception())
            return;
        if (spreadable) {
            VERIFY(arg.is_object());
            Object& obj = arg.as_object();
            size_t k = 0;
            auto length = length_of_array_like(global_object, obj);
            if (vm.exception())
                return;

            if (n + length > MAX_ARRAY_LIKE_INDEX) {
                vm.throw_exception<TypeError>(global_object, ErrorType::ArrayMaxSize);
                return;
            }
            while (k < length) {
                auto k_exists = obj.has_property(k);
                if (vm.exception())
                    return;
                if (k_exists) {
                    auto k_value = obj.get(k);
                    if (vm.exception())
                        return;
                    new_array->create_data_property_or_throw(n, k_value);
                    if (vm.exception())
                        return;
                }
                ++n;
                ++k;
            }
        } else {
            if (n >= MAX_ARRAY_LIKE_INDEX) {
                vm.throw_exception<TypeError>(global_object, ErrorType::ArrayMaxSize);
                return;
            }
            new_array->create_data_property_or_throw(n, arg);
            if (vm.exception())
                return;
            ++n;
        }
    };

    append_to_new_array(this_object);
    if (vm.exception())
        return {};

    for (size_t i = 0; i < vm.argument_count(); ++i) {
        auto argument = vm.argument(i);
        append_to_new_array(argument);
        if (vm.exception())
            return {};
    }

    new_array->set(vm.names.length, Value(n), Object::ShouldThrowExceptions::Yes);
    if (vm.exception())
        return {};
    return Value(new_array);
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::slice)
{
    auto* this_object = vm.this_value(global_object).to_object(global_object);
    if (!this_object)
        return {};

    auto initial_length = length_of_array_like(global_object, *this_object);
    if (vm.exception())
        return {};

    auto relative_start = vm.argument(0).to_integer_or_infinity(global_object);
    if (vm.exception())
        return {};

    double actual_start;

    if (Value(relative_start).is_negative_infinity())
        actual_start = 0.0;
    else if (relative_start < 0.0)
        actual_start = max((double)initial_length + relative_start, 0.0);
    else
        actual_start = min(relative_start, (double)initial_length);

    double relative_end;

    if (vm.argument(1).is_undefined() || vm.argument(1).is_empty()) {
        relative_end = (double)initial_length;
    } else {
        relative_end = vm.argument(1).to_integer_or_infinity(global_object);
        if (vm.exception())
            return {};
    }

    double final;

    if (Value(relative_end).is_negative_infinity())
        final = 0.0;
    else if (relative_end < 0.0)
        final = max((double)initial_length + relative_end, 0.0);
    else
        final = min(relative_end, (double)initial_length);

    auto count = max(final - actual_start, 0.0);

    auto* new_array = array_species_create(global_object, *this_object, count);
    if (vm.exception())
        return {};

    size_t index = 0;
    size_t k = actual_start;

    while (k < final) {
        bool present = this_object->has_property(k);
        if (vm.exception())
            return {};

        if (present) {
            auto value = this_object->get(k);
            if (vm.exception())
                return {};

            new_array->create_data_property_or_throw(index, value);
            if (vm.exception())
                return {};
        }

        ++k;
        ++index;
    }

    new_array->set(vm.names.length, Value(index), Object::ShouldThrowExceptions::Yes);
    if (vm.exception())
        return {};

    return new_array;
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::index_of)
{
    auto search_element = vm.argument(0);
    auto from_index = vm.argument(1);

    auto* object = vm.this_value(global_object).to_object(global_object);
    if (vm.exception())
        return {};

    auto length = length_of_array_like(global_object, *object);
    if (vm.exception())
        return {};

    if (length == 0)
        return Value(-1);

    auto n = from_index.to_integer_or_infinity(global_object);
    if (vm.exception())
        return {};

    if (from_index.is_undefined())
        VERIFY(n == 0);

    if (Value(n).is_positive_infinity())
        return Value(-1);

    if (Value(n).is_negative_infinity())
        n = 0;

    size_t k;

    if (n >= 0) {
        k = (size_t)n;
    }
    else {
        k = max(length + n, 0);
    }

    for (; k < length; ++k) {
        auto property_name = PropertyName { k };

        auto k_present = object->has_property(property_name);
        if (vm.exception())
            return {};

        if (k_present) {
            auto element_k = object->get(property_name);
            if (vm.exception())
                return {};

            auto same = strict_eq(search_element, element_k);

            if (same)
                return Value(k);
        }

    }

    return Value(-1);
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::reduce)
{
    auto callback_function = vm.argument(0);
    auto initial_value = vm.argument(1);

    auto* object = vm.this_value(global_object).to_object(global_object);
    if (vm.exception())
        return {};

    auto length = length_of_array_like(global_object, *object);
    if (vm.exception())
        return {};

    if (!callback_function.is_function()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAFunction, callback_function.to_string_without_side_effects());
        return {};
    }

    if (length == 0 && vm.argument_count() <= 1) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ReduceNoInitial);
        return {};
    }

    size_t k = 0;

    auto accumulator = js_undefined();

    if (vm.argument_count() > 1) {
        accumulator = initial_value;
    }
    else {
        bool k_present = false;

        for (; !k_present && k < length; ++k) {
            auto property_name = PropertyName { k };

            k_present = object->has_property(property_name);
            if (vm.exception())
                return {};

            if (k_present) {
                accumulator = object->get(property_name);
                if (vm.exception())
                    return {};
            }

        }

        if (!k_present) {
            vm.throw_exception<TypeError>(global_object, ErrorType::ReduceNoInitial);
            return {};
        }
    }

    for (; k < length; ++k) {
        auto property_name = PropertyName { k };

        auto k_present = object->has_property(property_name);

        if (k_present) {
            auto k_value = object->get(property_name);
            if (vm.exception())
                return {};

            accumulator = vm.call(callback_function.as_function(), js_undefined(), accumulator, k_value, Value(k), object);
            if (vm.exception())
                return {};
        }

    }

    return accumulator;
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::reduce_right)
{
    auto callback_function = vm.argument(0);
    auto initial_value = vm.argument(1);

    auto* object = vm.this_value(global_object).to_object(global_object);
    if (vm.exception())
        return {};

    auto length = length_of_array_like(global_object, *object);
    if (vm.exception())
        return {};

    if (!callback_function.is_function()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAFunction, callback_function.to_string_without_side_effects());
        return {};
    }

    if (length == 0 && vm.argument_count() <= 1) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ReduceNoInitial);
        return {};
    }

    ssize_t k = length - 1;

    auto accumulator = js_undefined();

    if (vm.argument_count() > 1) {
        accumulator = initial_value;
    }
    else {
        bool k_present = false;

        for (; !k_present && k >= 0; --k) {
            auto property_name = PropertyName { k };

            k_present = object->has_property(property_name);
            if (vm.exception())
                return {};

            if (k_present) {
                accumulator = object->get(property_name);
                if (vm.exception())
                    return {};
            }

        }

        if (!k_present) {
            vm.throw_exception<TypeError>(global_object, ErrorType::ReduceNoInitial);
            return {};
        }
    }

    for (; k >= 0; --k) {
        auto property_name = PropertyName { k };

        auto k_present = object->has_property(property_name);
        if (vm.exception())
            return {};

        if (k_present) {
            auto k_value = object->get(property_name);
            if (vm.exception())
                return {};

            accumulator = vm.call(callback_function.as_function(), js_undefined(), accumulator, k_value, Value((size_t)k), object);
            if (vm.exception())
                return {};
        }

    }

    return accumulator;
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::reverse)
{
    auto* this_object = vm.this_value(global_object).to_object(global_object);
    if (!this_object)
        return {};
    auto length = length_of_array_like(global_object, *this_object);
    if (vm.exception())
        return {};

    auto middle = length / 2;
    for (size_t lower = 0; lower < middle; ++lower) {
        auto upper = length - lower - 1;

        auto lower_exists = this_object->has_property(lower);
        if (vm.exception())
            return {};
        Value lower_value;
        if (lower_exists) {
            lower_value = this_object->get(lower);
            if (vm.exception())
                return {};
        }

        auto upper_exists = this_object->has_property(upper);
        if (vm.exception())
            return {};
        Value upper_value;
        if (upper_exists) {
            upper_value = this_object->get(upper);
            if (vm.exception())
                return {};
        }

        if (lower_exists && upper_exists) {
            this_object->set(lower, upper_value, Object::ShouldThrowExceptions::Yes);
            if (vm.exception())
                return {};
            this_object->set(upper, lower_value, Object::ShouldThrowExceptions::Yes);
            if (vm.exception())
                return {};
        } else if (!lower_exists && upper_exists) {
            this_object->set(lower, upper_value, Object::ShouldThrowExceptions::Yes);
            if (vm.exception())
                return {};
            this_object->delete_property_or_throw(upper);
            if (vm.exception())
                return {};
        } else if (lower_exists && !upper_exists) {
            this_object->delete_property_or_throw(lower);
            if (vm.exception())
                return {};
            this_object->set(upper, lower_value, Object::ShouldThrowExceptions::Yes);
            if (vm.exception())
                return {};
        }
    }

    return this_object;
}

static void array_merge_sort(VM& vm, GlobalObject& global_object, FunctionObject* compare_func, MarkedValueList& arr_to_sort)
{
    if (arr_to_sort.size() <= 1)
        return;

    MarkedValueList left(vm.heap());
    MarkedValueList right(vm.heap());

    left.ensure_capacity(arr_to_sort.size() / 2);
    right.ensure_capacity(arr_to_sort.size() / 2 + (arr_to_sort.size() & 1));

    for (size_t i = 0; i < arr_to_sort.size(); ++i) {
        if (i < arr_to_sort.size() / 2) {
            left.append(arr_to_sort[i]);
        } else {
            right.append(arr_to_sort[i]);
        }
    }

    array_merge_sort(vm, global_object, compare_func, left);
    if (vm.exception())
        return;
    array_merge_sort(vm, global_object, compare_func, right);
    if (vm.exception())
        return;

    arr_to_sort.clear();

    size_t left_index = 0, right_index = 0;

    while (left_index < left.size() && right_index < right.size()) {
        auto x = left[left_index];
        auto y = right[right_index];

        double comparison_result;

        if (x.is_undefined() && y.is_undefined()) {
            comparison_result = 0;
        } else if (x.is_undefined()) {
            comparison_result = 1;
        } else if (y.is_undefined()) {
            comparison_result = -1;
        } else if (compare_func) {
            auto call_result = vm.call(*compare_func, js_undefined(), left[left_index], right[right_index]);
            if (vm.exception())
                return;

            auto number = call_result.to_number(global_object);
            if (vm.exception())
                return;

            if (number.is_nan())
                comparison_result = 0;
            else
                comparison_result = number.as_double();
        } else {

            auto x_string = x.to_primitive_string(global_object);
            if (vm.exception())
                return;
            auto y_string = y.to_primitive_string(global_object);
            if (vm.exception())
                return;

            auto x_string_value = Value(x_string);
            auto y_string_value = Value(y_string);

            auto x_lt_y_relation = abstract_relation(global_object, true, x_string_value, y_string_value);
            VERIFY(x_lt_y_relation != TriState::Unknown);
            auto y_lt_x_relation = abstract_relation(global_object, true, y_string_value, x_string_value);
            VERIFY(y_lt_x_relation != TriState::Unknown);

            if (x_lt_y_relation == TriState::True) {
                comparison_result = -1;
            } else if (y_lt_x_relation == TriState::True) {
                comparison_result = 1;
            } else {
                comparison_result = 0;
            }
        }

        if (comparison_result <= 0) {
            arr_to_sort.append(left[left_index]);
            left_index++;
        } else {
            arr_to_sort.append(right[right_index]);
            right_index++;
        }
    }

    while (left_index < left.size()) {
        arr_to_sort.append(left[left_index]);
        left_index++;
    }

    while (right_index < right.size()) {
        arr_to_sort.append(right[right_index]);
        right_index++;
    }
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::sort)
{
    auto callback = vm.argument(0);
    if (!callback.is_undefined() && !callback.is_function()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAFunction, callback.to_string_without_side_effects());
        return {};
    }

    auto* object = vm.this_value(global_object).to_object(global_object);
    if (vm.exception())
        return {};

    auto length = length_of_array_like(global_object, *object);
    if (vm.exception())
        return {};

    MarkedValueList items(vm.heap());
    for (size_t k = 0; k < length; ++k) {
        auto k_present = object->has_property(k);
        if (vm.exception())
            return {};

        if (k_present) {
            auto k_value = object->get(k);
            if (vm.exception())
                return {};

            items.append(k_value);
        }
    }

    array_merge_sort(vm, global_object, callback.is_undefined() ? nullptr : &callback.as_function(), items);
    if (vm.exception())
        return {};

    for (size_t j = 0; j < items.size(); ++j) {
        object->set(j, items[j], Object::ShouldThrowExceptions::Yes);
        if (vm.exception())
            return {};
    }

    for (size_t j = items.size(); j < length; ++j) {
        object->delete_property_or_throw(j);
        if (vm.exception())
            return {};
    }

    return object;
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::last_index_of)
{
    auto search_element = vm.argument(0);
    auto from_index = vm.argument(1);

    auto* object = vm.this_value(global_object).to_object(global_object);
    if (vm.exception())
        return {};

    auto length = length_of_array_like(global_object, *object);
    if (vm.exception())
        return {};

    if (length == 0)
        return Value(-1);

    double n;

    if (vm.argument_count() >= 2) {
        n = from_index.to_integer_or_infinity(global_object);
        if (vm.exception())
            return {};
    } else {
        n = (double)length - 1;
    }

    if (Value(n).is_negative_infinity())
        return Value(-1);

    ssize_t k;

    if (n >= 0) {
        k = min(n, (double)length - 1);
    }
    else {
        k = (double)length + n;
    }

    for (; k >= 0; --k) {
        auto property_name = PropertyName { k };

        auto k_present = object->has_property(property_name);
        if (vm.exception())
            return {};

        if (k_present) {
            auto element_k = object->get(property_name);
            if (vm.exception())
                return {};

            auto same = strict_eq(search_element, element_k);

            if (same)
                return Value((size_t)k);
        }

    }

    return Value(-1);
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::includes)
{
    auto* this_object = vm.this_value(global_object).to_object(global_object);
    if (!this_object)
        return {};
    auto length = length_of_array_like(global_object, *this_object);
    if (vm.exception())
        return {};
    if (length == 0)
        return Value(false);
    u64 from_index = 0;
    if (vm.argument_count() >= 2) {
        auto from_argument = vm.argument(1).to_integer_or_infinity(global_object);
        if (vm.exception())
            return {};
        if (Value(from_argument).is_positive_infinity() || from_argument >= length)
            return Value(false);

        if (Value(from_argument).is_negative_infinity())
            from_argument = 0;

        if (from_argument < 0)
            from_index = max(length + from_argument, 0);
        else
            from_index = from_argument;
    }
    auto value_to_find = vm.argument(0);
    for (u64 i = from_index; i < length; ++i) {
        auto element = this_object->get(i);
        if (vm.exception())
            return {};
        if (same_value_zero(element, value_to_find))
            return Value(true);
    }
    return Value(false);
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::find)
{
    auto predicate = vm.argument(0);
    auto this_arg = vm.argument(1);

    auto* object = vm.this_value(global_object).to_object(global_object);
    if (vm.exception())
        return {};

    auto length = length_of_array_like(global_object, *object);
    if (vm.exception())
        return {};

    if (!predicate.is_function()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAFunction, predicate.to_string_without_side_effects());
        return {};
    }

    for (size_t k = 0; k < length; ++k) {
        auto property_name = PropertyName { k };

        auto k_value = object->get(property_name);
        if (vm.exception())
            return {};

        auto test_result = vm.call(predicate.as_function(), this_arg, k_value, Value(k), object);
        if (vm.exception())
            return {};

        if (test_result.to_boolean())
            return k_value;

    }

    return js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::find_index)
{
    auto predicate = vm.argument(0);
    auto this_arg = vm.argument(1);

    auto* object = vm.this_value(global_object).to_object(global_object);
    if (vm.exception())
        return {};

    auto length = length_of_array_like(global_object, *object);
    if (vm.exception())
        return {};

    if (!predicate.is_function()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAFunction, predicate.to_string_without_side_effects());
        return {};
    }

    for (size_t k = 0; k < length; ++k) {
        auto property_name = PropertyName { k };

        auto k_value = object->get(property_name);
        if (vm.exception())
            return {};

        auto test_result = vm.call(predicate.as_function(), this_arg, k_value, Value(k), object);
        if (vm.exception())
            return {};

        if (test_result.to_boolean())
            return Value(k);

    }

    return Value(-1);
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::find_last)
{
    auto predicate = vm.argument(0);
    auto this_arg = vm.argument(1);

    auto* object = vm.this_value(global_object).to_object(global_object);
    if (vm.exception())
        return {};

    auto length = length_of_array_like(global_object, *object);
    if (vm.exception())
        return {};

    if (!predicate.is_function()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAFunction, predicate.to_string_without_side_effects());
        return {};
    }

    for (i64 k = static_cast<i64>(length) - 1; k >= 0; --k) {
        auto property_name = PropertyName { k };

        auto k_value = object->get(property_name);
        if (vm.exception())
            return {};

        auto test_result = vm.call(predicate.as_function(), this_arg, k_value, Value((double)k), object);
        if (vm.exception())
            return {};

        if (test_result.to_boolean())
            return k_value;

    }

    return js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::find_last_index)
{
    auto predicate = vm.argument(0);
    auto this_arg = vm.argument(1);

    auto* object = vm.this_value(global_object).to_object(global_object);
    if (vm.exception())
        return {};

    auto length = length_of_array_like(global_object, *object);
    if (vm.exception())
        return {};

    if (!predicate.is_function()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAFunction, predicate.to_string_without_side_effects());
        return {};
    }

    for (i64 k = static_cast<i64>(length) - 1; k >= 0; --k) {
        auto property_name = PropertyName { k };

        auto k_value = object->get(property_name);
        if (vm.exception())
            return {};

        auto test_result = vm.call(predicate.as_function(), this_arg, k_value, Value((double)k), object);
        if (vm.exception())
            return {};

        if (test_result.to_boolean())
            return Value((double)k);

    }

    return Value(-1);
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::some)
{
    auto callback_function = vm.argument(0);
    auto this_arg = vm.argument(1);

    auto* object = vm.this_value(global_object).to_object(global_object);
    if (vm.exception())
        return {};

    auto length = length_of_array_like(global_object, *object);
    if (vm.exception())
        return {};

    if (!callback_function.is_function()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAFunction, callback_function.to_string_without_side_effects());
        return {};
    }

    for (size_t k = 0; k < length; ++k) {
        auto property_name = PropertyName { k };

        auto k_present = object->has_property(property_name);
        if (vm.exception())
            return {};

        if (k_present) {
            auto k_value = object->get(property_name);
            if (vm.exception())
                return {};

            auto test_result = vm.call(callback_function.as_function(), this_arg, k_value, Value(k), object);
            if (vm.exception())
                return {};

            if (test_result.to_boolean())
                return Value(true);
        }

    }

    return Value(false);
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::every)
{
    auto callback_function = vm.argument(0);
    auto this_arg = vm.argument(1);

    auto object = vm.this_value(global_object).to_object(global_object);
    if (vm.exception())
        return {};

    auto length = length_of_array_like(global_object, *object);
    if (vm.exception())
        return {};

    if (!callback_function.is_function()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAFunction, callback_function.to_string_without_side_effects());
        return {};
    }

    for (size_t k = 0; k < length; ++k) {
        auto property_name = PropertyName { k };

        auto k_present = object->has_property(property_name);
        if (vm.exception())
            return {};

        if (k_present) {
            auto k_value = object->get(property_name);
            if (vm.exception())
                return {};

            auto test_result = vm.call(callback_function.as_function(), this_arg, k_value, Value(k), object);
            if (vm.exception())
                return {};

            if (!test_result.to_boolean())
                return Value(false);
        }

    }

    return Value(true);
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::splice)
{
    auto* this_object = vm.this_value(global_object).to_object(global_object);
    if (!this_object)
        return {};

    auto initial_length = length_of_array_like(global_object, *this_object);
    if (vm.exception())
        return {};

    auto relative_start = vm.argument(0).to_integer_or_infinity(global_object);
    if (vm.exception())
        return {};

    if (Value(relative_start).is_negative_infinity())
        relative_start = 0;

    u64 actual_start;

    if (relative_start < 0)
        actual_start = max((ssize_t)initial_length + relative_start, (ssize_t)0);
    else
        actual_start = min(relative_start, initial_length);

    u64 insert_count = 0;
    double actual_delete_count = 0;

    if (vm.argument_count() == 1) {
        actual_delete_count = initial_length - actual_start;
    } else if (vm.argument_count() >= 2) {
        insert_count = vm.argument_count() - 2;
        auto delete_count = vm.argument(1).to_integer_or_infinity(global_object);
        if (vm.exception())
            return {};
        auto temp = max(delete_count, 0);
        actual_delete_count = min(temp, initial_length - actual_start);
    }

    double new_length = initial_length + insert_count - actual_delete_count;

    if (new_length > MAX_ARRAY_LIKE_INDEX) {
        vm.throw_exception<TypeError>(global_object, ErrorType::ArrayMaxSize);
        return {};
    }

    auto* removed_elements = array_species_create(global_object, *this_object, actual_delete_count);
    if (vm.exception())
        return {};

    for (u64 i = 0; i < actual_delete_count; ++i) {
        auto from = actual_start + i;
        bool from_present = this_object->has_property(from);
        if (vm.exception())
            return {};

        if (from_present) {
            auto from_value = this_object->get(from);
            if (vm.exception())
                return {};

            removed_elements->create_data_property_or_throw(i, from_value);
            if (vm.exception())
                return {};
        }
    }

    removed_elements->set(vm.names.length, Value(actual_delete_count), Object::ShouldThrowExceptions::Yes);
    if (vm.exception())
        return {};

    if (insert_count < actual_delete_count) {
        for (u64 i = actual_start; i < initial_length - actual_delete_count; ++i) {
            auto to = i + insert_count;
            u64 from = i + actual_delete_count;

            auto from_present = this_object->has_property(from);
            if (vm.exception())
                return {};

            if (from_present) {
                auto from_value = this_object->get(from);
                if (vm.exception())
                    return {};

                this_object->set(to, from_value, Object::ShouldThrowExceptions::Yes);
            } else {
                this_object->delete_property_or_throw(to);
            }
            if (vm.exception())
                return {};
        }

        for (u64 i = initial_length; i > new_length; --i) {
            this_object->delete_property_or_throw(i - 1);
            if (vm.exception())
                return {};
        }
    } else if (insert_count > actual_delete_count) {
        for (u64 i = initial_length - actual_delete_count; i > actual_start; --i) {
            u64 from_index = i + actual_delete_count - 1;
            auto from_present = this_object->has_property(from_index);
            if (vm.exception())
                return {};

            auto to = i + insert_count - 1;

            if (from_present) {
                auto from_value = this_object->get(from_index);
                if (vm.exception())
                    return {};
                this_object->set(to, from_value, Object::ShouldThrowExceptions::Yes);
            } else {
                this_object->delete_property_or_throw(to);
            }
            if (vm.exception())
                return {};
        }
    }

    for (u64 i = 0; i < insert_count; ++i) {
        this_object->set(actual_start + i, vm.argument(i + 2), Object::ShouldThrowExceptions::Yes);
        if (vm.exception())
            return {};
    }

    this_object->set(vm.names.length, Value(new_length), Object::ShouldThrowExceptions::Yes);
    if (vm.exception())
        return {};

    return removed_elements;
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::fill)
{
    auto* this_object = vm.this_value(global_object).to_object(global_object);
    if (!this_object)
        return {};

    auto length = length_of_array_like(global_object, *this_object);
    if (vm.exception())
        return {};

    double relative_start = 0;
    double relative_end = length;

    if (vm.argument_count() >= 2) {
        relative_start = vm.argument(1).to_integer_or_infinity(global_object);
        if (vm.exception())
            return {};
        if (Value(relative_start).is_negative_infinity())
            relative_start = 0;
    }

    if (vm.argument_count() >= 3 && !vm.argument(2).is_undefined()) {
        relative_end = vm.argument(2).to_integer_or_infinity(global_object);
        if (vm.exception())
            return {};
        if (Value(relative_end).is_negative_infinity())
            relative_end = 0;
    }

    u64 from, to;

    if (relative_start < 0)
        from = max(length + relative_start, 0L);
    else
        from = min(relative_start, length);

    if (relative_end < 0)
        to = max(length + relative_end, 0L);
    else
        to = min(relative_end, length);

    for (u64 i = from; i < to; i++) {
        this_object->set(i, vm.argument(0), Object::ShouldThrowExceptions::Yes);
        if (vm.exception())
            return {};
    }

    return this_object;
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::values)
{
    auto* this_object = vm.this_value(global_object).to_object(global_object);
    if (!this_object)
        return {};

    return ArrayIterator::create(global_object, this_object, Object::PropertyKind::Value);
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::entries)
{
    auto* this_object = vm.this_value(global_object).to_object(global_object);
    if (!this_object)
        return {};

    return ArrayIterator::create(global_object, this_object, Object::PropertyKind::KeyAndValue);
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::keys)
{
    auto* this_object = vm.this_value(global_object).to_object(global_object);
    if (!this_object)
        return {};

    return ArrayIterator::create(global_object, this_object, Object::PropertyKind::Key);
}

static size_t flatten_into_array(GlobalObject& global_object, Object& new_array, Object& array, size_t array_length, size_t target_index, double depth, FunctionObject* mapper_func = {}, Value this_arg = {})
{
    VERIFY(!mapper_func || (!this_arg.is_empty() && depth == 1));
    auto& vm = global_object.vm();

    for (size_t j = 0; j < array_length; ++j) {
        auto value_exists = array.has_property(j);
        if (vm.exception())
            return {};

        if (!value_exists)
            continue;
        auto value = array.get(j);
        if (vm.exception())
            return {};

        if (mapper_func) {
            value = vm.call(*mapper_func, this_arg, value, Value(j), &array);
            if (vm.exception())
                return {};
        }

        if (depth > 0 && value.is_array(global_object)) {
            auto length = length_of_array_like(global_object, value.as_object());
            if (vm.exception())
                return {};
            target_index = flatten_into_array(global_object, new_array, value.as_object(), length, target_index, depth - 1);
            if (vm.exception())
                return {};
            continue;
        }

        if (target_index >= MAX_ARRAY_LIKE_INDEX) {
            vm.throw_exception<TypeError>(global_object, ErrorType::InvalidIndex);
            return {};
        }

        new_array.create_data_property_or_throw(target_index, value);
        if (vm.exception())
            return {};

        ++target_index;
    }
    return target_index;
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::flat)
{
    auto* this_object = vm.this_value(global_object).to_object(global_object);
    if (!this_object)
        return {};

    auto length = length_of_array_like(global_object, *this_object);
    if (vm.exception())
        return {};

    double depth = 1;
    if (!vm.argument(0).is_undefined()) {
        auto depth_num = vm.argument(0).to_integer_or_infinity(global_object);
        if (vm.exception())
            return {};
        depth = max(depth_num, 0.0);
    }

    auto* new_array = array_species_create(global_object, *this_object, 0);
    if (vm.exception())
        return {};

    flatten_into_array(global_object, *new_array, *this_object, length, 0, depth);
    if (vm.exception())
        return {};
    return new_array;
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::flat_map)
{
    auto mapper_function = vm.argument(0);
    auto this_arg = vm.argument(1);

    auto* object = vm.this_value(global_object).to_object(global_object);
    if (vm.exception())
        return {};

    auto source_length = length_of_array_like(global_object, *object);
    if (vm.exception())
        return {};

    if (!mapper_function.is_function()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAFunction, mapper_function.to_string_without_side_effects());
        return {};
    }

    auto* array = array_species_create(global_object, *object, 0);
    if (vm.exception())
        return {};

    flatten_into_array(global_object, *array, *object, source_length, 0, 1, &mapper_function.as_function(), this_arg);
    if (vm.exception())
        return {};

    return array;
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::copy_within)
{
    auto* this_object = vm.this_value(global_object).to_object(global_object);
    if (!this_object)
        return {};
    auto length = length_of_array_like(global_object, *this_object);
    if (vm.exception())
        return {};

    auto relative_target = vm.argument(0).to_integer_or_infinity(global_object);
    if (vm.exception())
        return {};

    double to;
    if (relative_target < 0)
        to = max(length + relative_target, 0.0);
    else
        to = min(relative_target, (double)length);

    auto relative_start = vm.argument(1).to_integer_or_infinity(global_object);
    if (vm.exception())
        return {};

    double from;
    if (relative_start < 0)
        from = max(length + relative_start, 0.0);
    else
        from = min(relative_start, (double)length);

    auto relative_end = vm.argument(2).is_undefined() ? length : vm.argument(2).to_integer_or_infinity(global_object);
    if (vm.exception())
        return {};

    double final;
    if (relative_end < 0)
        final = max(length + relative_end, 0.0);
    else
        final = min(relative_end, (double)length);

    double count = min(final - from, length - to);

    i32 direction = 1;

    if (from < to && to < from + count) {
        direction = -1;
        from = from + count - 1;
        to = to + count - 1;
    }

    if (count < 0) {
        return this_object;
    }

    size_t from_i = from;
    size_t to_i = to;
    size_t count_i = count;

    while (count_i > 0) {
        auto from_present = this_object->has_property(from_i);
        if (vm.exception())
            return {};

        if (from_present) {
            auto from_value = this_object->get(from_i);
            if (vm.exception())
                return {};
            this_object->set(to_i, from_value, Object::ShouldThrowExceptions::Yes);
            if (vm.exception())
                return {};
        } else {
            this_object->delete_property_or_throw(to_i);
            if (vm.exception())
                return {};
        }

        from_i += direction;
        to_i += direction;
        --count_i;
    }

    return this_object;
}

JS_DEFINE_NATIVE_FUNCTION(ArrayPrototype::at)
{
    auto* this_object = vm.this_value(global_object).to_object(global_object);
    if (!this_object)
        return {};
    auto length = length_of_array_like(global_object, *this_object);
    if (vm.exception())
        return {};
    auto relative_index = vm.argument(0).to_integer_or_infinity(global_object);
    if (vm.exception())
        return {};
    if (Value(relative_index).is_infinity())
        return js_undefined();
    Checked<size_t> index { 0 };
    if (relative_index >= 0) {
        index += relative_index;
    } else {
        index += length;
        index -= -relative_index;
    }
    if (index.has_overflow() || index.value() >= length)
        return js_undefined();
    return this_object->get(index.value());
}

}
