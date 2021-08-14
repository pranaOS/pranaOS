/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/ErrorTypes.h>

namespace JS {

#define __ENUMERATE_JS_ERROR(name, message) \
    const ErrorType ErrorType::name = ErrorType(message);
JS_ENUMERATE_ERROR_TYPES(__ENUMERATE_JS_ERROR)
#undef __ENUMERATE_JS_ERROR

}
