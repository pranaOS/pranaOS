/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/Optional.h>
#include <libjs/Forward.h>
#include <libjs/Runtime/Value.h>

namespace JS {

Value from_property_descriptor(GlobalObject&, Optional<PropertyDescriptor> const&);
PropertyDescriptor to_property_descriptor(GlobalObject&, Value);

class PropertyDescriptor {
public:
    [[nodiscard]] bool is_accessor_descriptor() const;
    [[nodiscard]] bool is_data_descriptor() const;
    [[nodiscard]] bool is_generic_descriptor() const;

    [[nodiscard]] PropertyAttributes attributes() const;

    void complete();

    [[nodiscard]] bool is_empty() const
    {
        return !value.has_value() && !get.has_value() && !set.has_value() && !writable.has_value() && !enumerable.has_value() && !configurable.has_value();
    }

    Optional<Value> value {};
    Optional<FunctionObject*> get {};
    Optional<FunctionObject*> set {};
    Optional<bool> writable {};
    Optional<bool> enumerable {};
    Optional<bool> configurable {};
};

}

namespace Base {

template<>
struct Formatter<JS::PropertyDescriptor> : Formatter<StringView> {
    void format(FormatBuilder& builder, JS::PropertyDescriptor const& property_descriptor)
    {
        Vector<String> parts;
        if (property_descriptor.value.has_value())
            parts.append(String::formatted("[[Value]]: {}", property_descriptor.value->to_string_without_side_effects()));
        if (property_descriptor.get.has_value())
            parts.append(String::formatted("[[Get]]: JS::Function* @ {:p}", *property_descriptor.get));
        if (property_descriptor.set.has_value())
            parts.append(String::formatted("[[Set]]: JS::Function* @ {:p}", *property_descriptor.set));
        if (property_descriptor.writable.has_value())
            parts.append(String::formatted("[[Writable]]: {}", *property_descriptor.writable));
        if (property_descriptor.enumerable.has_value())
            parts.append(String::formatted("[[Enumerable]]: {}", *property_descriptor.enumerable));
        if (property_descriptor.configurable.has_value())
            parts.append(String::formatted("[[Configurable]]: {}", *property_descriptor.configurable));
        Formatter<StringView>::format(builder, String::formatted("PropertyDescriptor {{ {} }}", String::join(", ", parts)));
    }
};

}
