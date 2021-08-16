/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/Weakable.h>
#include <libjs/Forward.h>
#include <libweb/Forward.h>

namespace Web::Bindings {

class ScriptExecutionContext {
public:
    virtual ~ScriptExecutionContext();

    virtual JS::Interpreter& interpreter() = 0;
};

}
