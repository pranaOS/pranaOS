/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <libjs/runtime/AbstractOperations.h>
#include <libjs/runtime/ArrayBuffer.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/PropertyDescriptor.h>
#include <libjs/runtime/PropertyName.h>
#include <libjs/runtime/TypedArrayConstructor.h>
#include <libjs/runtime/VM.h>

namespace JS {

class TypedArrayBase;

TypedArrayBase* typed_array_from(GlobalObject&, Value);
void validate_typed_array(GlobalObject&, TypedArrayBase&);

class TypedArrayBase : public Object {
    JS_OBJECT(TypedArrayBase, Object);

public:
    enum class ContentType {
        BigInt,
        Number,
    };

    u32 array_length() const { return m_array_length; }
    u32 byte_length() const { return m_byte_length; }
    u32 byte_offset() const { return m_byte_offset; }
    ContentType content_type() const { return m_content_type; }
    ArrayBuffer* viewed_array_buffer() const { return m_viewed_array_buffer; }

    void set_array_length(u32 length) { m_array_length = length; }
    void set_byte_length(u32 length) { m_byte_length = length; }
    void set_byte_offset(u32 offset) { m_byte_offset = offset; }
    void set_viewed_array_buffer(ArrayBuffer* array_buffer) { m_viewed_array_buffer = array_buffer; }

    virtual size_t element_size() const = 0;
    virtual String element_name() const = 0;

    virtual bool is_unclamped_integer_element_type() const = 0;

    virtual bool is_bigint_element_type() const = 0;

    virtual Value get_value_from_buffer(size_t byte_index, ArrayBuffer::Order, bool is_little_endian = true) const = 0;
    virtual void set_value_in_buffer(size_t byte_index, Value, ArrayBuffer::Order, bool is_little_endian = true) = 0;

    virtual Value get_modify_set_value_in_buffer(size_t byte_index, Value value, ReadWriteModifyFunction operation, bool is_little_endian = true) = 0;

protected:
    explicit TypedArrayBase(Object& prototype)
        : Object(prototype)
    {
    }

    u32 m_array_length { 0 };
    u32 m_byte_length { 0 };
    u32 m_byte_offset { 0 };
    ContentType m_content_type { ContentType::Number };
    ArrayBuffer* m_viewed_array_buffer { nullptr };

private:
    virtual void visit_edges(Visitor&) override;
};

inline bool is_valid_integer_index(TypedArrayBase const& typed_array, Value property_index)
{
    if (typed_array.viewed_array_buffer()->is_detached())
        return false;


    if (!property_index.is_integral_number())
        return false;
    if (property_index.is_negative_zero())
        return false;

    if (property_index.as_double() < 0 || property_index.as_double() >= typed_array.array_length())
        return false;

    return true;
}

template<typename T>
inline Value integer_indexed_element_get(TypedArrayBase const& typed_array, Value property_index)
{

    if (!is_valid_integer_index(typed_array, property_index))
        return js_undefined();

    auto offset = typed_array.byte_offset();

    Checked<size_t> indexed_position = (i64)property_index.as_double();
    indexed_position *= typed_array.element_size();
    indexed_position += offset;
    if (indexed_position.has_overflow()) {
        dbgln("integer_indexed_element_get(): indexed_position overflowed, returning as if it's an invalid index.");
        return js_undefined();
    }

    return typed_array.viewed_array_buffer()->template get_value<T>(indexed_position.value(), true, ArrayBuffer::Order::Unordered);
}

template<typename T>
inline void integer_indexed_element_set(TypedArrayBase& typed_array, Value property_index, Value value)
{
    VERIFY(!value.is_empty());
    auto& vm = typed_array.vm();
    auto& global_object = typed_array.global_object();


    Value num_value;

    if (typed_array.content_type() == TypedArrayBase::ContentType::BigInt) {
        num_value = value.to_bigint(global_object);
        if (vm.exception())
            return;
    }
    else {
        num_value = value.to_number(global_object);
        if (vm.exception())
            return;
    }

    if (!is_valid_integer_index(typed_array, property_index))
        return;

    auto offset = typed_array.byte_offset();

    Checked<size_t> indexed_position = (i64)property_index.as_double();
    indexed_position *= typed_array.element_size();
    indexed_position += offset;
    if (indexed_position.has_overflow()) {
        dbgln("integer_indexed_element_set(): indexed_position overflowed, returning as if succeeded.");
        return;
    }

    typed_array.viewed_array_buffer()->template set_value<T>(indexed_position.value(), num_value, true, ArrayBuffer::Order::Unordered);

}

template<typename T>
class TypedArray : public TypedArrayBase {
    JS_OBJECT(TypedArray, TypedArrayBase);

    using UnderlyingBufferDataType = Conditional<IsSame<ClampedU8, T>, u8, T>;

public:
    virtual Optional<PropertyDescriptor> internal_get_own_property(PropertyName const& property_name) const override
    {
        VERIFY(property_name.is_valid());



        if (property_name.is_string() || property_name.is_number()) {
            auto numeric_index = canonical_numeric_index_string(global_object(), property_name);
            if (!numeric_index.is_undefined()) {
                auto value = integer_indexed_element_get<T>(*this, numeric_index);

                if (value.is_undefined())
                    return {};

                return PropertyDescriptor {
                    .value = value,
                    .writable = true,
                    .enumerable = true,
                    .configurable = true,
                };
            }
        }

        return Object::internal_get_own_property(property_name);
    }

    virtual bool internal_has_property(PropertyName const& property_name) const override
    {
        VERIFY(property_name.is_valid());



        if (property_name.is_string() || property_name.is_number()) {
            auto numeric_index = canonical_numeric_index_string(global_object(), property_name);
            if (!numeric_index.is_undefined())
                return is_valid_integer_index(*this, numeric_index);
        }

        return Object::internal_has_property(property_name);
    }

    virtual bool internal_define_own_property(PropertyName const& property_name, PropertyDescriptor const& property_descriptor) override
    {
        VERIFY(property_name.is_valid());



        if (property_name.is_string() || property_name.is_number()) {
            auto numeric_index = canonical_numeric_index_string(global_object(), property_name);
            if (!numeric_index.is_undefined()) {
                if (!is_valid_integer_index(*this, numeric_index))
                    return false;

                if (property_descriptor.configurable.has_value() && !*property_descriptor.configurable)
                    return false;

                if (property_descriptor.enumerable.has_value() && !*property_descriptor.enumerable)
                    return false;

                if (property_descriptor.is_accessor_descriptor())
                    return false;

                if (property_descriptor.writable.has_value() && !*property_descriptor.writable)
                    return false;

                if (property_descriptor.value.has_value()) {
                    integer_indexed_element_set<T>(*this, numeric_index, *property_descriptor.value);
                    if (vm().exception())
                        return {};
                }

                return true;
            }
        }

        return Object::internal_define_own_property(property_name, property_descriptor);
    }

    virtual Value internal_get(PropertyName const& property_name, Value receiver) const override
    {
        VERIFY(!receiver.is_empty());

        VERIFY(property_name.is_valid());

        if (property_name.is_string() || property_name.is_number()) {
            auto numeric_index = canonical_numeric_index_string(global_object(), property_name);
            if (!numeric_index.is_undefined()) {
                return integer_indexed_element_get<T>(*this, numeric_index);
            }
        }

        return Object::internal_get(property_name, receiver);
    }

    virtual bool internal_set(PropertyName const& property_name, Value value, Value receiver) override
    {
        VERIFY(!value.is_empty());
        VERIFY(!receiver.is_empty());

        VERIFY(property_name.is_valid());

        if (property_name.is_string() || property_name.is_number()) {
            auto numeric_index = canonical_numeric_index_string(global_object(), property_name);
            if (!numeric_index.is_undefined()) {
                integer_indexed_element_set<T>(*this, numeric_index, value);
                if (vm().exception())
                    return {};

                return true;
            }
        }

        return Object::internal_set(property_name, value, receiver);
    }

    virtual bool internal_delete(PropertyName const& property_name) override
    {
        VERIFY(property_name.is_valid());


        if (property_name.is_string() || property_name.is_number()) {
            auto numeric_index = canonical_numeric_index_string(global_object(), property_name);
            if (!numeric_index.is_undefined()) {
                if (!is_valid_integer_index(*this, numeric_index))
                    return true;
                return false;
            }
        }

        return Object::internal_delete(property_name);
    }

    virtual MarkedValueList internal_own_property_keys() const override
    {
        auto& vm = this->vm();

        auto keys = MarkedValueList { heap() };


        if (!m_viewed_array_buffer->is_detached()) {
            for (size_t i = 0; i < m_array_length; ++i) {
                keys.append(js_string(vm, String::number(i)));
            }
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

    Span<const UnderlyingBufferDataType> data() const
    {
        return { reinterpret_cast<const UnderlyingBufferDataType*>(m_viewed_array_buffer->buffer().data() + m_byte_offset), m_array_length };
    }
    Span<UnderlyingBufferDataType> data()
    {
        return { reinterpret_cast<UnderlyingBufferDataType*>(m_viewed_array_buffer->buffer().data() + m_byte_offset), m_array_length };
    }

    virtual size_t element_size() const override { return sizeof(UnderlyingBufferDataType); };

    bool is_unclamped_integer_element_type() const override
    {
        constexpr bool is_unclamped_integer = IsSame<T, i8> || IsSame<T, u8> || IsSame<T, i16> || IsSame<T, u16> || IsSame<T, i32> || IsSame<T, u32>;
        return is_unclamped_integer;
    }

    bool is_bigint_element_type() const override
    {
        constexpr bool is_bigint = IsSame<T, i64> || IsSame<T, u64>;
        return is_bigint;
    }

    Value get_value_from_buffer(size_t byte_index, ArrayBuffer::Order order, bool is_little_endian = true) const override { return viewed_array_buffer()->template get_value<T>(byte_index, true, order, is_little_endian); }
    void set_value_in_buffer(size_t byte_index, Value value, ArrayBuffer::Order order, bool is_little_endian = true) override { viewed_array_buffer()->template set_value<T>(byte_index, value, true, order, is_little_endian); }
    Value get_modify_set_value_in_buffer(size_t byte_index, Value value, ReadWriteModifyFunction operation, bool is_little_endian = true) override { return viewed_array_buffer()->template get_modify_set_value<T>(byte_index, value, move(operation), is_little_endian); }

protected:
    TypedArray(u32 array_length, Object& prototype)
        : TypedArrayBase(prototype)
    {
        VERIFY(!Checked<u32>::multiplication_would_overflow(array_length, sizeof(UnderlyingBufferDataType)));
        m_viewed_array_buffer = ArrayBuffer::create(global_object(), array_length * sizeof(UnderlyingBufferDataType));
        if (array_length)
            VERIFY(!data().is_null());
        m_array_length = array_length;
        m_byte_length = m_viewed_array_buffer->byte_length();
    }

private:
    virtual bool is_typed_array() const final { return true; }
};

TypedArrayBase* typed_array_create(GlobalObject& global_object, FunctionObject& constructor, MarkedValueList arguments);

#define JS_DECLARE_TYPED_ARRAY(ClassName, snake_name, PrototypeName, ConstructorName, Type) \
    class ClassName : public TypedArray<Type> {                                             \
        JS_OBJECT(ClassName, TypedArray);                                                   \
                                                                                            \
    public:                                                                                 \
        virtual ~ClassName();                                                               \
        static ClassName* create(GlobalObject&, u32 length, FunctionObject& new_target);    \
        static ClassName* create(GlobalObject&, u32 length);                                \
        ClassName(u32 length, Object& prototype);                                           \
        virtual String element_name() const override;                                       \
    };                                                                                      \
    class PrototypeName final : public Object {                                             \
        JS_OBJECT(PrototypeName, Object);                                                   \
                                                                                            \
    public:                                                                                 \
        PrototypeName(GlobalObject&);                                                       \
        virtual void initialize(GlobalObject&) override;                                    \
        virtual ~PrototypeName() override;                                                  \
    };                                                                                      \
    class ConstructorName final : public TypedArrayConstructor {                            \
        JS_OBJECT(ConstructorName, TypedArrayConstructor);                                  \
                                                                                            \
    public:                                                                                 \
        explicit ConstructorName(GlobalObject&);                                            \
        virtual void initialize(GlobalObject&) override;                                    \
        virtual ~ConstructorName() override;                                                \
                                                                                            \
        virtual Value call() override;                                                      \
        virtual Value construct(FunctionObject& new_target) override;                       \
                                                                                            \
    private:                                                                                \
        virtual bool has_constructor() const override { return true; }                      \
    };

#define __JS_ENUMERATE(ClassName, snake_name, PrototypeName, ConstructorName, Type) \
    JS_DECLARE_TYPED_ARRAY(ClassName, snake_name, PrototypeName, ConstructorName, Type);
JS_ENUMERATE_TYPED_ARRAYS
#undef __JS_ENUMERATE

}
