/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/AggregateError.h>
#include <libjs/runtime/Error.h>
#include <libjs/runtime/GlobalObject.h>

namespace JS {

AggregateError* AggregateError::create(GlobalObject& global_object)
{
    return global_object.heap().allocate<AggregateError>(global_object, *global_object.aggregate_error_prototype());
}

AggregateError::AggregateError(Object& prototype)
    : Error(prototype)
{
}

}
