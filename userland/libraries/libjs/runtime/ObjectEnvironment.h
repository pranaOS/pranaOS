/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/Environment.h>

namespace JS {

class ObjectEnvironment : public Environment {
    JS_ENVIRONMENT(ObjectEnvironment, Environment);

public:
    enum class IsWithEnvironment {
        No,
        Yes,
    };
    ObjectEnvironment(Object& binding_object, IsWithEnvironment, Environment* outer_environment);

    virtual Optional<Variable> get_from_environment(FlyString const&) const override;
    virtual bool put_into_environment(FlyString const&, Variable) override;
    virtual bool delete_from_environment(FlyString const&) override;

    virtual bool has_binding(FlyString const& name) const override;
    virtual void create_mutable_binding(GlobalObject&, FlyString const& name, bool can_be_deleted) override;
    virtual void create_immutable_binding(GlobalObject&, FlyString const& name, bool strict) override;
    virtual void initialize_binding(GlobalObject&, FlyString const& name, Value) override;
    virtual void set_mutable_binding(GlobalObject&, FlyString const& name, Value, bool strict) override;
    virtual Value get_binding_value(GlobalObject&, FlyString const& name, bool strict) override;
    virtual bool delete_binding(GlobalObject&, FlyString const& name) override;

    virtual Object* with_base_object() const override
    {
        if (is_with_environment())
            return &m_binding_object;
        return nullptr;
    }

    Object& binding_object() { return m_binding_object; }


    bool is_with_environment() const { return m_with_environment; }

private:
    virtual void visit_edges(Visitor&) override;

    Object& m_binding_object;
    bool m_with_environment { false };
};

}
