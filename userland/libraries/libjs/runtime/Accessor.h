/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/FunctionObject.h>
#include <libjs/runtime/VM.h>

namespace JS {

class Accessor final : public Cell {
public:
    static Accessor* create(VM& vm, FunctionObject* getter, FunctionObject* setter)
    {
        return vm.heap().allocate_without_global_object<Accessor>(getter, setter);
    }

    Accessor(FunctionObject* getter, FunctionObject* setter)
        : m_getter(getter)
        , m_setter(setter)
    {
    }

    FunctionObject* getter() const { return m_getter; }
    void set_getter(FunctionObject* getter) { m_getter = getter; }

    FunctionObject* setter() const { return m_setter; }
    void set_setter(FunctionObject* setter) { m_setter = setter; }

    Value call_getter(Value this_value)
    {
        if (!m_getter)
            return js_undefined();
        return vm().call(*m_getter, this_value);
    }

    void call_setter(Value this_value, Value setter_value)
    {
        if (!m_setter)
            return;
        [[maybe_unused]] auto rc = vm().call(*m_setter, this_value, setter_value);
    }

    void visit_edges(Cell::Visitor& visitor) override
    {
        visitor.visit(m_getter);
        visitor.visit(m_setter);
    }

private:
    const char* class_name() const override { return "Accessor"; };

    FunctionObject* m_getter { nullptr };
    FunctionObject* m_setter { nullptr };
};

}
