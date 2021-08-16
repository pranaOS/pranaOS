/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/Error.h>
#include <libjs/runtime/Object.h>

namespace JS {

class AggregateError : public Error {
    JS_OBJECT(AggregateError, Error);

public:
    static AggregateError* create(GlobalObject&);

    explicit AggregateError(Object& prototype);
    virtual ~AggregateError() override = default;
};

}
