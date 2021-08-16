/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/FlyString.h>
#include <base/Vector.h>
#include <libjs/heap/Cell.h>
#include <libjs/runtime/Value.h>
#include <libjs/SourceRange.h>

namespace JS {

struct TracebackFrame {
    FlyString function_name;
    SourceRange source_range;
};

class Exception : public Cell {
public:
    explicit Exception(Value);
    virtual ~Exception() override = default;

    Value value() const { return m_value; }
    const Vector<TracebackFrame, 32>& traceback() const { return m_traceback; }

private:
    virtual const char* class_name() const override { return "Exception"; }
    virtual void visit_edges(Visitor&) override;

    Value m_value;
    Vector<TracebackFrame, 32> m_traceback;
};

}
