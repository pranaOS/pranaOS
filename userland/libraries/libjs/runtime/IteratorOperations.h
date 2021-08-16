/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/Function.h>
#include <libjs/runtime/Object.h>

namespace JS {

enum class IteratorHint {
    Sync,
    Async,
};

Object* get_iterator(GlobalObject&, Value value, IteratorHint hint = IteratorHint::Sync, Value method = {});
Object* iterator_next(Object& iterator, Value value = {});
Object* iterator_step(GlobalObject&, Object& iterator);
bool iterator_complete(GlobalObject&, Object& iterator_result);
Value iterator_value(GlobalObject&, Object& iterator_result);
void iterator_close(Object& iterator);
Value create_iterator_result_object(GlobalObject&, Value value, bool done);
MarkedValueList iterable_to_list(GlobalObject&, Value iterable, Value method = {});

enum class CloseOnAbrupt {
    No,
    Yes
};
void get_iterator_values(GlobalObject&, Value value, Function<IterationDecision(Value)> callback, Value method = {}, CloseOnAbrupt close_on_abrupt = CloseOnAbrupt::Yes);

}
