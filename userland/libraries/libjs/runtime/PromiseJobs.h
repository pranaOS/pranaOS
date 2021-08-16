/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/JobCallback.h>
#include <libjs/runtime/NativeFunction.h>
#include <libjs/runtime/Promise.h>

namespace JS {

class PromiseReactionJob final : public NativeFunction {
    JS_OBJECT(PromiseReactionJob, NativeFunction);

public:
    static PromiseReactionJob* create(GlobalObject&, PromiseReaction&, Value argument);

    explicit PromiseReactionJob(PromiseReaction&, Value argument, Object& prototype);
    virtual ~PromiseReactionJob() override = default;

    virtual Value call() override;

private:
    virtual void visit_edges(Visitor&) override;

    PromiseReaction& m_reaction;
    Value m_argument;
};

class PromiseResolveThenableJob final : public NativeFunction {
    JS_OBJECT(PromiseReactionJob, NativeFunction);

public:
    static PromiseResolveThenableJob* create(GlobalObject&, Promise&, Value thenable, JobCallback then);

    explicit PromiseResolveThenableJob(Promise&, Value thenable, JobCallback then, Object& prototype);
    virtual ~PromiseResolveThenableJob() override = default;

    virtual Value call() override;

private:
    virtual void visit_edges(Visitor&) override;

    Promise& m_promise_to_resolve;
    Value m_thenable;
    JobCallback m_then;
};

}
