/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/JobCallback.h>
#include <libjs/runtime/VM.h>

namespace JS {

struct PromiseCapability {
    Object* promise { nullptr };
    FunctionObject* resolve { nullptr };
    FunctionObject* reject { nullptr };
};

PromiseCapability new_promise_capability(GlobalObject& global_object, Value constructor);

class PromiseReaction final : public Cell {
public:
    enum class Type {
        Fulfill,
        Reject,
    };

    static PromiseReaction* create(VM& vm, Type type, Optional<PromiseCapability> capability, Optional<JobCallback> handler)
    {
        return vm.heap().allocate_without_global_object<PromiseReaction>(type, capability, handler);
    }

    PromiseReaction(Type type, Optional<PromiseCapability> capability, Optional<JobCallback> handler);
    virtual ~PromiseReaction() = default;

    Type type() const { return m_type; }
    const Optional<PromiseCapability>& capability() const { return m_capability; }
    const Optional<JobCallback>& handler() const { return m_handler; }

private:
    virtual const char* class_name() const override { return "PromiseReaction"; }
    virtual void visit_edges(Visitor&) override;

    Type m_type;
    Optional<PromiseCapability> m_capability;
    Optional<JobCallback> m_handler;
};

}
