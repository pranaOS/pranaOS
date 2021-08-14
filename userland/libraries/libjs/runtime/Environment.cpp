/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/Environment.h>
#include <libjs/runtime/VM.h>

namespace JS {

Environment::Environment(Environment* outer_environment)
    : m_outer_environment(outer_environment)
{
}

void Environment::initialize(GlobalObject& global_object)
{
    m_global_object = &global_object;
    Cell::initialize(global_object);
}

void Environment::visit_edges(Visitor& visitor)
{
    Cell::visit_edges(visitor);
    visitor.visit(m_outer_environment);
}

}
