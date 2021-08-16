/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/String.h>
#include <libjs/runtime/Environment.h>
#include <libjs/runtime/PropertyName.h>
#include <libjs/runtime/Value.h>

namespace JS {

class Reference {
public:
    enum class BaseType : u8 {
        Unresolvable,
        Value,
        Environment,
    };

    Reference() { }
    Reference(BaseType type, PropertyName const& name, bool strict)
        : m_base_type(type)
        , m_name(name)
        , m_strict(strict)
    {
    }

    Reference(Value base, PropertyName const& name, Value this_value, bool strict = false)
        : m_base_type(BaseType::Value)
        , m_base_value(base)
        , m_name(name)
        , m_this_value(this_value)
        , m_strict(strict)
    {
        if (base.is_nullish()) {
            m_base_type = BaseType::Unresolvable;
            m_base_value = {};
            m_this_value = {};
            m_name = {};
        }
    }

    Reference(Environment& base, FlyString const& referenced_name, bool strict = false)
        : m_base_type(BaseType::Environment)
        , m_base_environment(&base)
        , m_name(referenced_name)
        , m_strict(strict)
    {
    }

    Value base() const
    {
        VERIFY(m_base_type == BaseType::Value);
        return m_base_value;
    }

    Environment& base_environment() const
    {
        VERIFY(m_base_type == BaseType::Environment);
        return *m_base_environment;
    }

    PropertyName const& name() const { return m_name; }
    bool is_strict() const { return m_strict; }

    bool is_unresolvable() const { return m_base_type == BaseType::Unresolvable; }

    bool is_property_reference() const
    {
        if (is_unresolvable())
            return false;
        if (m_base_type == BaseType::Environment)
            return false;
        if (m_base_value.is_boolean() || m_base_value.is_string() || m_base_value.is_symbol() || m_base_value.is_bigint() || m_base_value.is_number() || m_base_value.is_object())
            return true;
        return false;
    }

    Value get_this_value() const
    {
        VERIFY(is_property_reference());
        if (is_super_reference())
            return m_this_value;
        return m_base_value;
    }

    bool is_super_reference() const
    {
        return !m_this_value.is_empty();
    }

    void put_value(GlobalObject&, Value);
    Value get_value(GlobalObject&, bool throw_if_undefined = true);
    bool delete_(GlobalObject&);

    String to_string() const;

private:
    void throw_reference_error(GlobalObject&);

    BaseType m_base_type { BaseType::Unresolvable };
    union {
        Value m_base_value {};
        Environment* m_base_environment;
    };
    PropertyName m_name;
    Value m_this_value;
    bool m_strict { false };
};

}
