/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/String.h>
#include <libjs/Interpreter.h>
#include <libjs/runtime/AbstractOperations.h>
#include <libjs/runtime/Accessor.h>
#include <libjs/runtime/Array.h>
#include <libjs/runtime/Error.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/NativeFunction.h>
#include <libjs/runtime/Object.h>
#include <libjs/runtime/PropertyDescriptor.h>
#include <libjs/runtime/ProxyObject.h>
#include <libjs/runtime/Shape.h>
#include <libjs/runtime/TemporaryClearException.h>
#include <libjs/runtime/Value.h>

namespace JS {

Object* Object::create(GlobalObject& global_object, Object* prototype)
{
    if (!prototype)
        return global_object.heap().allocate<Object>(global_object, *global_object.empty_object_shape());
    else if (prototype == global_object.object_prototype())
        return global_object.heap().allocate<Object>(global_object, *global_object.new_object_shape());
    else
        return global_object.heap().allocate<Object>(global_object, *prototype);
}

Object::Object(GlobalObjectTag)
{
    m_shape = heap().allocate_without_global_object<Shape>(*this);
}

Object::Object(ConstructWithoutPrototypeTag, GlobalObject& global_object)
{
    m_shape = heap().allocate_without_global_object<Shape>(global_object);
}

Object::Object(Object& prototype)
{
    m_shape = prototype.global_object().empty_object_shape();
    auto success = internal_set_prototype_of(&prototype);
    VERIFY(success);
}

Object::Object(Shape& shape)
    : m_shape(&shape)
{
    m_storage.resize(shape.property_count());
}

void Object::initialize(GlobalObject&)
{
}

Object::~Object()
{
}

bool Object::is_extensible() const
{
    return internal_is_extensible();
}

Value Object::get(PropertyName const& property_name) const
{
    VERIFY(property_name.is_valid());

    return internal_get(property_name, this);
}

bool Object::set(PropertyName const& property_name, Value value, ShouldThrowExceptions throw_exceptions)
{
    VERIFY(!value.is_empty());
    auto& vm = this->vm();

    VERIFY(property_name.is_valid());


    auto success = internal_set(property_name, value, this);
    if (vm.exception())
        return {};

    if (!success && throw_exceptions == ShouldThrowExceptions::Yes) {
        vm.throw_exception<TypeError>(global_object(), ErrorType::ObjectSetReturnedFalse);
        return {};
    }

    return success;
}

bool Object::create_data_property(PropertyName const& property_name, Value value)
{

    VERIFY(property_name.is_valid());

    auto new_descriptor = PropertyDescriptor {
        .value = value,
        .writable = true,
        .enumerable = true,
        .configurable = true,
    };

    return internal_define_own_property(property_name, new_descriptor);
}

bool Object::create_method_property(PropertyName const& property_name, Value value)
{
    VERIFY(!value.is_empty());


    VERIFY(property_name.is_valid());

    auto new_descriptor = PropertyDescriptor {
        .value = value,
        .writable = true,
        .enumerable = false,
        .configurable = true,
    };

    return internal_define_own_property(property_name, new_descriptor);
}

bool Object::create_data_property_or_throw(PropertyName const& property_name, Value value)
{
    VERIFY(!value.is_empty());
    auto& vm = this->vm();


    VERIFY(property_name.is_valid());

    auto success = create_data_property(property_name, value);
    if (vm.exception())
        return {};

    if (!success) {
        vm.throw_exception<TypeError>(global_object(), ErrorType::ObjectDefineOwnPropertyReturnedFalse);
        return {};
    }

    return success;
}

bool Object::create_non_enumerable_data_property_or_throw(PropertyName const& property_name, Value value)
{
    VERIFY(!value.is_empty());
    VERIFY(property_name.is_valid());

    auto new_description = PropertyDescriptor { .value = value, .writable = true, .enumerable = false, .configurable = true };

    return define_property_or_throw(property_name, new_description);
}

bool Object::define_property_or_throw(PropertyName const& property_name, PropertyDescriptor const& property_descriptor)
{
    auto& vm = this->vm();


    VERIFY(property_name.is_valid());

    auto success = internal_define_own_property(property_name, property_descriptor);
    if (vm.exception())
        return {};

    if (!success) {
        vm.throw_exception<TypeError>(global_object(), ErrorType::ObjectDefineOwnPropertyReturnedFalse);
        return {};
    }

    return success;
}

bool Object::delete_property_or_throw(PropertyName const& property_name)
{
    auto& vm = this->vm();


    VERIFY(property_name.is_valid());

    auto success = internal_delete(property_name);
    if (vm.exception())
        return {};

    if (!success) {
        vm.throw_exception<TypeError>(global_object(), ErrorType::ObjectDeleteReturnedFalse);
        return {};
    }

    return success;
}

bool Object::has_property(PropertyName const& property_name) const
{

    VERIFY(property_name.is_valid());

    return internal_has_property(property_name);
}

bool Object::has_own_property(PropertyName const& property_name) const
{
    auto& vm = this->vm();


    VERIFY(property_name.is_valid());

    auto descriptor = internal_get_own_property(property_name);
    if (vm.exception())
        return {};

    if (!descriptor.has_value())
        return false;

    return true;
}

bool Object::set_integrity_level(IntegrityLevel level)
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();


    VERIFY(level == IntegrityLevel::Sealed || level == IntegrityLevel::Frozen);

    auto status = internal_prevent_extensions();
    if (vm.exception())
        return {};

    if (!status)
        return false;

    auto keys = internal_own_property_keys();
    if (vm.exception())
        return {};

    if (level == IntegrityLevel::Sealed) {
        for (auto& key : keys) {
            auto property_name = PropertyName::from_value(global_object, key);

            define_property_or_throw(property_name, { .configurable = false });
            if (vm.exception())
                return {};
        }
    }
    else {

        for (auto& key : keys) {
            auto property_name = PropertyName::from_value(global_object, key);

            auto current_descriptor = internal_get_own_property(property_name);
            if (vm.exception())
                return {};

            if (!current_descriptor.has_value())
                continue;

            PropertyDescriptor descriptor;

            if (current_descriptor->is_accessor_descriptor()) {
                descriptor = { .configurable = false };
            }
            else {
                descriptor = { .writable = false, .configurable = false };
            }

            define_property_or_throw(property_name, descriptor);
            if (vm.exception())
                return {};
        }
    }

    return true;
}

bool Object::test_integrity_level(IntegrityLevel level) const
{
    auto& vm = this->vm();


    VERIFY(level == IntegrityLevel::Sealed || level == IntegrityLevel::Frozen);

    auto extensible = is_extensible();
    if (vm.exception())
        return {};

    if (extensible)
        return false;

    auto keys = internal_own_property_keys();
    if (vm.exception())
        return {};

    for (auto& key : keys) {
        auto property_name = PropertyName::from_value(global_object(), key);

        auto current_descriptor = internal_get_own_property(property_name);
        if (vm.exception())
            return {};

        if (!current_descriptor.has_value())
            continue;
        if (*current_descriptor->configurable)
            return false;

        if (level == IntegrityLevel::Frozen && current_descriptor->is_data_descriptor()) {
            if (*current_descriptor->writable)
                return false;
        }
    }

    return true;
}

MarkedValueList Object::enumerable_own_property_names(PropertyKind kind) const
{

    auto& vm = this->vm();
    auto& global_object = this->global_object();


    auto own_keys = internal_own_property_keys();
    if (vm.exception())
        return MarkedValueList { heap() };

    auto properties = MarkedValueList { heap() };

    for (auto& key : own_keys) {
        if (!key.is_string())
            continue;
        auto property_name = PropertyName::from_value(global_object, key);

        auto descriptor = internal_get_own_property(property_name);
        if (vm.exception())
            return MarkedValueList { heap() };

        if (descriptor.has_value() && *descriptor->enumerable) {
            if (kind == PropertyKind::Key) {
                properties.append(key);
                continue;
            }

            auto value = get(property_name);
            if (vm.exception())
                return MarkedValueList { heap() };

            if (kind == PropertyKind::Value) {
                properties.append(value);
                continue;
            }

            VERIFY(kind == PropertyKind::KeyAndValue);

            auto entry = Array::create_from(global_object, { key, value });

            properties.append(entry);
        }
    }

    return properties;
}


Object* Object::internal_get_prototype_of() const
{
    return const_cast<Object*>(prototype());
}

bool Object::internal_set_prototype_of(Object* new_prototype)
{

    if (prototype() == new_prototype)
        return true;

    if (!m_is_extensible)
        return false;

    auto* prototype = new_prototype;

    while (prototype) {

        if (prototype == this)
            return false;

        if (is<ProxyObject>(prototype))
            break;

        prototype = prototype->prototype();
    }

    auto& shape = this->shape();
    if (shape.is_unique())
        shape.set_prototype_without_transition(new_prototype);
    else
        m_shape = shape.create_prototype_transition(new_prototype);

    return true;
}

bool Object::internal_is_extensible() const
{
    return m_is_extensible;
}

bool Object::internal_prevent_extensions()
{
    m_is_extensible = false;

    return true;
}

Optional<PropertyDescriptor> Object::internal_get_own_property(PropertyName const& property_name) const
{
    VERIFY(property_name.is_valid());

    if (!storage_has(property_name))
        return {};

    PropertyDescriptor descriptor;

    auto [value, attributes] = *storage_get(property_name);

    if (!value.is_accessor()) {
        descriptor.value = value.value_or(js_undefined());

        descriptor.writable = attributes.is_writable();
    }
    else {

        descriptor.get = value.as_accessor().getter();

        descriptor.set = value.as_accessor().setter();
    }

    descriptor.enumerable = attributes.is_enumerable();

    descriptor.configurable = attributes.is_configurable();

    return descriptor;
}

bool Object::internal_define_own_property(PropertyName const& property_name, PropertyDescriptor const& property_descriptor)
{
    VERIFY(property_name.is_valid());
    auto& vm = this->vm();

    auto current = internal_get_own_property(property_name);
    if (vm.exception())
        return {};

    auto extensible = is_extensible();
    if (vm.exception())
        return {};

    return validate_and_apply_property_descriptor(this, property_name, extensible, property_descriptor, current);
}

bool Object::internal_has_property(PropertyName const& property_name) const
{
    auto& vm = this->vm();

    VERIFY(property_name.is_valid());

    auto has_own = internal_get_own_property(property_name);
    if (vm.exception())
        return {};

    if (has_own.has_value())
        return true;

    auto parent = internal_get_prototype_of();
    if (vm.exception())
        return {};

    if (parent) {
        return parent->internal_has_property(property_name);
    }

    return false;
}

Value Object::internal_get(PropertyName const& property_name, Value receiver) const
{
    VERIFY(!receiver.is_empty());
    auto& vm = this->vm();

    VERIFY(property_name.is_valid());

    auto descriptor = internal_get_own_property(property_name);
    if (vm.exception())
        return {};

    if (!descriptor.has_value()) {
        auto parent = internal_get_prototype_of();
        if (vm.exception())
            return {};

        if (!parent)
            return js_undefined();

        return parent->internal_get(property_name, receiver);
    }

    if (descriptor->is_data_descriptor())
        return *descriptor->value;

    VERIFY(descriptor->is_accessor_descriptor());

    auto* getter = *descriptor->get;

    if (!getter)
        return js_undefined();

    return vm.call(*getter, receiver);
}

static bool ordinary_set_with_own_descriptor(Object&, PropertyName const&, Value, Value, Optional<PropertyDescriptor>);

bool Object::internal_set(PropertyName const& property_name, Value value, Value receiver)
{
    VERIFY(!value.is_empty());
    VERIFY(!receiver.is_empty());
    auto& vm = this->vm();

    VERIFY(property_name.is_valid());

    auto own_descriptor = internal_get_own_property(property_name);
    if (vm.exception())
        return {};

    return ordinary_set_with_own_descriptor(*this, property_name, value, receiver, own_descriptor);
}

bool ordinary_set_with_own_descriptor(Object& object, PropertyName const& property_name, Value value, Value receiver, Optional<PropertyDescriptor> own_descriptor)
{
    auto& vm = object.vm();

    VERIFY(property_name.is_valid());

    if (!own_descriptor.has_value()) {
        auto parent = object.internal_get_prototype_of();
        if (vm.exception())
            return {};

        if (parent) {
            return parent->internal_set(property_name, value, receiver);
        }
        else {
            own_descriptor = PropertyDescriptor {
                .value = js_undefined(),
                .writable = true,
                .enumerable = true,
                .configurable = true,
            };
        }
    }

    if (own_descriptor->is_data_descriptor()) {
        if (!*own_descriptor->writable)
            return false;

        if (!receiver.is_object())
            return false;

        auto existing_descriptor = receiver.as_object().internal_get_own_property(property_name);
        if (vm.exception())
            return {};

        if (existing_descriptor.has_value()) {
            if (existing_descriptor->is_accessor_descriptor())
                return false;

            if (!*existing_descriptor->writable)
                return false;

            auto value_descriptor = PropertyDescriptor { .value = value };

            return receiver.as_object().internal_define_own_property(property_name, value_descriptor);
        }
        else {
            VERIFY(!receiver.as_object().storage_has(property_name));

            return receiver.as_object().create_data_property(property_name, value);
        }
    }

    VERIFY(own_descriptor->is_accessor_descriptor());

    auto* setter = *own_descriptor->set;

    if (!setter)
        return false;

    (void)vm.call(*setter, receiver, value);

    return true;
}

bool Object::internal_delete(PropertyName const& property_name)
{
    auto& vm = this->vm();

    VERIFY(property_name.is_valid());

    auto descriptor = internal_get_own_property(property_name);
    if (vm.exception())
        return {};

    if (!descriptor.has_value())
        return true;

    if (*descriptor->configurable) {
        storage_delete(property_name);

        return true;
    }

    return false;
}

MarkedValueList Object::internal_own_property_keys() const
{
    auto& vm = this->vm();

    MarkedValueList keys { heap() };

    for (auto& entry : m_indexed_properties) {
        keys.append(js_string(vm, String::number(entry.index())));
    }

    for (auto& it : shape().property_table_ordered()) {
        if (it.key.is_string()) {
            keys.append(it.key.to_value(vm));
        }
    }

    for (auto& it : shape().property_table_ordered()) {
        if (it.key.is_symbol()) {
            keys.append(it.key.to_value(vm));
        }
    }

    return keys;
}

bool Object::set_immutable_prototype(Object* prototype)
{
    auto& vm = this->vm();


    auto* current = internal_get_prototype_of();
    if (vm.exception())
        return {};

    if (prototype == current)
        return true;

    return false;
}

Optional<ValueAndAttributes> Object::storage_get(PropertyName const& property_name) const
{
    VERIFY(property_name.is_valid());

    Value value;
    PropertyAttributes attributes;

    if (property_name.is_number()) {
        auto value_and_attributes = m_indexed_properties.get(property_name.as_number());
        if (!value_and_attributes.has_value())
            return {};
        value = value_and_attributes->value;
        attributes = value_and_attributes->attributes;
    } else {
        auto metadata = shape().lookup(property_name.to_string_or_symbol());
        if (!metadata.has_value())
            return {};
        value = m_storage[metadata->offset];
        attributes = metadata->attributes;
    }
    return ValueAndAttributes { .value = value, .attributes = attributes };
}

bool Object::storage_has(PropertyName const& property_name) const
{
    VERIFY(property_name.is_valid());
    if (property_name.is_number())
        return m_indexed_properties.has_index(property_name.as_number());
    return shape().lookup(property_name.to_string_or_symbol()).has_value();
}

void Object::storage_set(PropertyName const& property_name, ValueAndAttributes const& value_and_attributes)
{
    VERIFY(property_name.is_valid());

    auto [value, attributes] = value_and_attributes;

    if (property_name.is_number()) {
        auto index = property_name.as_number();
        m_indexed_properties.put(index, value, attributes);
        return;
    }

    if (!m_transitions_enabled && !m_shape->is_unique()) {
        m_shape->add_property_without_transition(property_name, attributes);
        m_storage.resize(m_shape->property_count());
        m_storage[m_shape->property_count() - 1] = value;
        return;
    }

    auto property_name_string_or_symbol = property_name.to_string_or_symbol();
    auto metadata = shape().lookup(property_name_string_or_symbol);

    if (!metadata.has_value()) {
        if (!m_shape->is_unique() && shape().property_count() > 100) {
            ensure_shape_is_unique();
        }

        if (m_shape->is_unique()) {
            m_shape->add_property_to_unique_shape(property_name_string_or_symbol, attributes);
            m_storage.resize(m_shape->property_count());
        } else if (m_transitions_enabled) {
            set_shape(*m_shape->create_put_transition(property_name_string_or_symbol, attributes));
        } else {
            m_shape->add_property_without_transition(property_name, attributes);
            m_storage.resize(m_shape->property_count());
        }
        metadata = shape().lookup(property_name_string_or_symbol);
        VERIFY(metadata.has_value());
    }

    if (attributes != metadata->attributes) {
        if (m_shape->is_unique()) {
            m_shape->reconfigure_property_in_unique_shape(property_name_string_or_symbol, attributes);
        } else {
            set_shape(*m_shape->create_configure_transition(property_name_string_or_symbol, attributes));
        }
        metadata = shape().lookup(property_name_string_or_symbol);
        VERIFY(metadata.has_value());
    }

    m_storage[metadata->offset] = value;
}

void Object::storage_delete(PropertyName const& property_name)
{
    VERIFY(property_name.is_valid());
    VERIFY(storage_has(property_name));

    if (property_name.is_number())
        return m_indexed_properties.remove(property_name.as_number());

    auto metadata = shape().lookup(property_name.to_string_or_symbol());
    VERIFY(metadata.has_value());

    ensure_shape_is_unique();

    shape().remove_property_from_unique_shape(property_name.to_string_or_symbol(), metadata->offset);
    m_storage.remove(metadata->offset);
}

void Object::set_shape(Shape& new_shape)
{
    m_storage.resize(new_shape.property_count());
    m_shape = &new_shape;
}

void Object::define_native_accessor(PropertyName const& property_name, Function<Value(VM&, GlobalObject&)> getter, Function<Value(VM&, GlobalObject&)> setter, PropertyAttributes attribute)
{
    auto& vm = this->vm();
    String formatted_property_name;
    if (property_name.is_number()) {
        formatted_property_name = property_name.to_string();
    } else if (property_name.is_string()) {
        formatted_property_name = property_name.as_string();
    } else {
        formatted_property_name = String::formatted("[{}]", property_name.as_symbol()->description());
    }
    FunctionObject* getter_function = nullptr;
    if (getter) {
        auto name = String::formatted("get {}", formatted_property_name);
        getter_function = NativeFunction::create(global_object(), name, move(getter));
        getter_function->define_direct_property(vm.names.length, Value(0), Attribute::Configurable);
        getter_function->define_direct_property(vm.names.name, js_string(vm, name), Attribute::Configurable);
    }
    FunctionObject* setter_function = nullptr;
    if (setter) {
        auto name = String::formatted("set {}", formatted_property_name);
        setter_function = NativeFunction::create(global_object(), name, move(setter));
        setter_function->define_direct_property(vm.names.length, Value(1), Attribute::Configurable);
        setter_function->define_direct_property(vm.names.name, js_string(vm, name), Attribute::Configurable);
    }
    return define_direct_accessor(property_name, getter_function, setter_function, attribute);
}

void Object::define_direct_accessor(PropertyName const& property_name, FunctionObject* getter, FunctionObject* setter, PropertyAttributes attributes)
{
    VERIFY(property_name.is_valid());

    auto existing_property = storage_get(property_name).value_or({}).value;
    auto* accessor = existing_property.is_accessor() ? &existing_property.as_accessor() : nullptr;
    if (!accessor) {
        accessor = Accessor::create(vm(), getter, setter);
        define_direct_property(property_name, accessor, attributes);
    } else {
        if (getter)
            accessor->set_getter(getter);
        if (setter)
            accessor->set_setter(setter);
    }
}

void Object::ensure_shape_is_unique()
{
    if (shape().is_unique())
        return;

    m_shape = m_shape->create_unique_clone();
}

Value Object::get_without_side_effects(const PropertyName& property_name) const
{
    auto* object = this;
    while (object) {
        auto value_and_attributes = object->storage_get(property_name);
        if (value_and_attributes.has_value())
            return value_and_attributes->value;
        object = object->prototype();
    }
    return {};
}

void Object::define_native_function(PropertyName const& property_name, Function<Value(VM&, GlobalObject&)> native_function, i32 length, PropertyAttributes attribute)
{
    auto& vm = this->vm();
    String function_name;
    if (property_name.is_string()) {
        function_name = property_name.as_string();
    } else {
        function_name = String::formatted("[{}]", property_name.as_symbol()->description());
    }
    auto* function = NativeFunction::create(global_object(), function_name, move(native_function));
    function->define_direct_property(vm.names.length, Value(length), Attribute::Configurable);
    function->define_direct_property(vm.names.name, js_string(vm, function_name), Attribute::Configurable);
    define_direct_property(property_name, function, attribute);
}

Object* Object::define_properties(Value properties)
{
    auto& vm = this->vm();
    auto& global_object = this->global_object();


    auto* props = properties.to_object(global_object);
    if (vm.exception())
        return {};

    auto keys = props->internal_own_property_keys();
    if (vm.exception())
        return {};

    struct NameAndDescriptor {
        PropertyName name;
        PropertyDescriptor descriptor;
    };

    Vector<NameAndDescriptor> descriptors;

    for (auto& next_key : keys) {
        auto property_name = PropertyName::from_value(global_object, next_key);

        auto property_descriptor = props->internal_get_own_property(property_name);
        if (vm.exception())
            return {};

        if (property_descriptor.has_value() && *property_descriptor->enumerable) {
            auto descriptor_object = props->get(property_name);
            if (vm.exception())
                return {};

            auto descriptor = to_property_descriptor(global_object, descriptor_object);
            if (vm.exception())
                return {};

            descriptors.append({ property_name, descriptor });
        }
    }

    for (auto& [name, descriptor] : descriptors) {

        define_property_or_throw(name, descriptor);
        if (vm.exception())
            return {};
    }

    return this;
}

void Object::visit_edges(Cell::Visitor& visitor)
{
    Cell::visit_edges(visitor);
    visitor.visit(m_shape);

    for (auto& value : m_storage)
        visitor.visit(value);

    m_indexed_properties.for_each_value([&visitor](auto& value) {
        visitor.visit(value);
    });
}

Value Object::ordinary_to_primitive(Value::PreferredType preferred_type) const
{
    VERIFY(preferred_type == Value::PreferredType::String || preferred_type == Value::PreferredType::Number);

    auto& vm = this->vm();

    Base::Array<PropertyName, 2> method_names;
    if (preferred_type == Value::PreferredType::String)
        method_names = { vm.names.toString, vm.names.valueOf };
    else
        method_names = { vm.names.valueOf, vm.names.toString };

    for (auto& method_name : method_names) {
        auto method = get(method_name);
        if (vm.exception())
            return {};
        if (method.is_function()) {
            auto result = vm.call(method.as_function(), const_cast<Object*>(this));
            if (!result.is_object())
                return result;
        }
    }
    vm.throw_exception<TypeError>(global_object(), ErrorType::Convert, "object", preferred_type == Value::PreferredType::String ? "string" : "number");
    return {};
}

}
