/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/HashMap.h>
#include <base/String.h>
#include <libjs/Forward.h>
#include <libjs/heap/Cell.h>
#include <libjs/runtime/IndexedProperties.h>
#include <libjs/runtime/MarkedValueList.h>
#include <libjs/runtime/PrimitiveString.h>
#include <libjs/runtime/PropertyDescriptor.h>
#include <libjs/runtime/PropertyName.h>
#include <libjs/runtime/Shape.h>
#include <libjs/runtime/Value.h>

namespace JS {

#define JS_OBJECT(class_, base_class) \
public:                               \
    using Base = base_class;          \
    virtual const char* class_name() const override { return #class_; }

class Object : public Cell {
public:
    static Object* create(GlobalObject&, Object* prototype);

    explicit Object(Object& prototype);
    explicit Object(Shape&);
    virtual void initialize(GlobalObject&) override;
    virtual ~Object();

    enum class PropertyKind {
        Key,
        Value,
        KeyAndValue,
    };

    enum class IntegrityLevel {
        Sealed,
        Frozen,
    };

    enum class ShouldThrowExceptions {
        No,
        Yes,
    };

    Value ordinary_to_primitive(Value::PreferredType preferred_type) const;

    bool is_extensible() const;

    Value get(PropertyName const&) const;
    bool set(PropertyName const&, Value, ShouldThrowExceptions);
    bool create_data_property(PropertyName const&, Value);
    bool create_method_property(PropertyName const&, Value);
    bool create_data_property_or_throw(PropertyName const&, Value);
    bool create_non_enumerable_data_property_or_throw(PropertyName const&, Value);
    bool define_property_or_throw(PropertyName const&, PropertyDescriptor const&);
    bool delete_property_or_throw(PropertyName const&);
    bool has_property(PropertyName const&) const;
    bool has_own_property(PropertyName const&) const;
    bool set_integrity_level(IntegrityLevel);
    bool test_integrity_level(IntegrityLevel) const;
    MarkedValueList enumerable_own_property_names(PropertyKind kind) const;


    virtual Object* internal_get_prototype_of() const;
    virtual bool internal_set_prototype_of(Object* prototype);
    virtual bool internal_is_extensible() const;
    virtual bool internal_prevent_extensions();
    virtual Optional<PropertyDescriptor> internal_get_own_property(PropertyName const&) const;
    virtual bool internal_define_own_property(PropertyName const&, PropertyDescriptor const&);
    virtual bool internal_has_property(PropertyName const&) const;
    virtual Value internal_get(PropertyName const&, Value receiver) const;
    virtual bool internal_set(PropertyName const&, Value value, Value receiver);
    virtual bool internal_delete(PropertyName const&);
    virtual MarkedValueList internal_own_property_keys() const;

    bool set_immutable_prototype(Object* prototype);

    Object* define_properties(Value properties);


    Optional<ValueAndAttributes> storage_get(PropertyName const&) const;
    bool storage_has(PropertyName const&) const;
    void storage_set(PropertyName const&, ValueAndAttributes const&);
    void storage_delete(PropertyName const&);


    Value get_without_side_effects(const PropertyName&) const;

    void define_direct_property(PropertyName const& property_name, Value value, PropertyAttributes attributes) { storage_set(property_name, { value, attributes }); };
    void define_direct_accessor(PropertyName const&, FunctionObject* getter, FunctionObject* setter, PropertyAttributes attributes);

    void define_native_function(PropertyName const&, Function<Value(VM&, GlobalObject&)>, i32 length, PropertyAttributes attributes);
    void define_native_accessor(PropertyName const&, Function<Value(VM&, GlobalObject&)> getter, Function<Value(VM&, GlobalObject&)> setter, PropertyAttributes attributes);

    virtual bool is_function() const { return false; }
    virtual bool is_typed_array() const { return false; }
    virtual bool is_string_object() const { return false; }
    virtual bool is_global_object() const { return false; }
    virtual bool is_proxy_object() const { return false; }
    virtual bool is_native_function() const { return false; }
    virtual bool is_ordinary_function_object() const { return false; }

    virtual bool is_htmldda() const { return false; }

    bool has_parameter_map() const { return m_has_parameter_map; }
    void set_has_parameter_map() { m_has_parameter_map = true; }

    virtual const char* class_name() const override { return "Object"; }
    virtual void visit_edges(Cell::Visitor&) override;
    virtual Value value_of() const { return Value(const_cast<Object*>(this)); }

    Value get_direct(size_t index) const { return m_storage[index]; }

    const IndexedProperties& indexed_properties() const { return m_indexed_properties; }
    IndexedProperties& indexed_properties() { return m_indexed_properties; }
    void set_indexed_property_elements(Vector<Value>&& values) { m_indexed_properties = IndexedProperties(move(values)); }

    Shape& shape() { return *m_shape; }
    Shape const& shape() const { return *m_shape; }

    GlobalObject& global_object() const { return *shape().global_object(); }

    void ensure_shape_is_unique();

    void enable_transitions() { m_transitions_enabled = true; }
    void disable_transitions() { m_transitions_enabled = false; }

    template<typename T>
    bool fast_is() const = delete;

protected:
    enum class GlobalObjectTag { Tag };
    enum class ConstructWithoutPrototypeTag { Tag };
    explicit Object(GlobalObjectTag);
    Object(ConstructWithoutPrototypeTag, GlobalObject&);

    bool m_is_extensible { true };

    bool m_has_parameter_map { false };

private:
    void set_shape(Shape&);

    Object* prototype() { return shape().prototype(); }
    Object const* prototype() const { return shape().prototype(); }

    bool m_transitions_enabled { true };
    Shape* m_shape { nullptr };
    Vector<Value> m_storage;
    IndexedProperties m_indexed_properties;
};

}
