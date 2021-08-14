/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/HashMap.h>
#include <base/TypeCasts.h>
#include <libjs/runtime/MapIterator.h>
#include <libjs/runtime/MapPrototype.h>

namespace JS {

MapPrototype::MapPrototype(GlobalObject& global_object)
    : Object(*global_object.object_prototype())
{
}

void MapPrototype::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    Object::initialize(global_object);
    u8 attr = Attribute::Writable | Attribute::Configurable;

    define_native_function(vm.names.clear, clear, 0, attr);
    define_native_function(vm.names.delete_, delete_, 1, attr);
    define_native_function(vm.names.entries, entries, 0, attr);
    define_native_function(vm.names.forEach, for_each, 1, attr);
    define_native_function(vm.names.get, get, 1, attr);
    define_native_function(vm.names.has, has, 1, attr);
    define_native_function(vm.names.keys, keys, 0, attr);
    define_native_function(vm.names.set, set, 2, attr);
    define_native_function(vm.names.values, values, 0, attr);

    define_native_accessor(vm.names.size, size_getter, {}, Attribute::Configurable);

    define_direct_property(*vm.well_known_symbol_iterator(), Object::get(vm.names.entries), attr);
    define_direct_property(*vm.well_known_symbol_to_string_tag(), js_string(global_object.heap(), vm.names.Map.as_string()), Attribute::Configurable);
}

MapPrototype::~MapPrototype()
{
}

Map* MapPrototype::typed_this(VM& vm, GlobalObject& global_object)
{
    auto* this_object = vm.this_value(global_object).to_object(global_object);
    if (!this_object)
        return {};
    if (!is<Map>(this_object)) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotA, "Map");
        return nullptr;
    }
    return static_cast<Map*>(this_object);
}

JS_DEFINE_NATIVE_FUNCTION(MapPrototype::clear)
{
    auto* map = typed_this(vm, global_object);
    if (!map)
        return {};
    map->entries().clear();
    return js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(MapPrototype::delete_)
{
    auto* map = typed_this(vm, global_object);
    if (!map)
        return {};
    return Value(map->entries().remove(vm.argument(0)));
}

JS_DEFINE_NATIVE_FUNCTION(MapPrototype::entries)
{
    auto* map = typed_this(vm, global_object);
    if (!map)
        return {};

    return MapIterator::create(global_object, *map, Object::PropertyKind::KeyAndValue);
}

JS_DEFINE_NATIVE_FUNCTION(MapPrototype::for_each)
{
    auto* map = typed_this(vm, global_object);
    if (!map)
        return {};
    if (!vm.argument(0).is_function()) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotAFunction, vm.argument(0).to_string_without_side_effects());
        return {};
    }
    auto this_value = vm.this_value(global_object);
    for (auto& entry : map->entries()) {
        (void)vm.call(vm.argument(0).as_function(), vm.argument(1), entry.value, entry.key, this_value);
        if (vm.exception())
            return {};
    }
    return js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(MapPrototype::get)
{
    auto* map = typed_this(vm, global_object);
    if (!map)
        return {};
    auto result = map->entries().get(vm.argument(0));
    if (!result.has_value())
        return js_undefined();
    return result.value();
}

JS_DEFINE_NATIVE_FUNCTION(MapPrototype::has)
{
    auto* map = typed_this(vm, global_object);
    if (!map)
        return {};
    auto& entries = map->entries();
    return Value(entries.find(vm.argument(0)) != entries.end());
}

JS_DEFINE_NATIVE_FUNCTION(MapPrototype::keys)
{
    auto* map = typed_this(vm, global_object);
    if (!map)
        return {};

    return MapIterator::create(global_object, *map, Object::PropertyKind::Key);
}

JS_DEFINE_NATIVE_FUNCTION(MapPrototype::set)
{
    auto* map = typed_this(vm, global_object);
    if (!map)
        return {};
    auto key = vm.argument(0);
    if (key.is_negative_zero())
        key = Value(0);
    map->entries().set(key, vm.argument(1));
    return map;
}

JS_DEFINE_NATIVE_FUNCTION(MapPrototype::values)
{
    auto* map = typed_this(vm, global_object);
    if (!map)
        return {};

    return MapIterator::create(global_object, *map, Object::PropertyKind::Value);
}

JS_DEFINE_NATIVE_GETTER(MapPrototype::size_getter)
{
    auto* map = typed_this(vm, global_object);
    if (!map)
        return {};
    return Value(map->entries().size());
}

}
