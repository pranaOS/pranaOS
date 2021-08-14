/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/runtime/FunctionObject.h>
#include <libjs/runtime/VM.h>

namespace JS {

struct JobCallback {
    FunctionObject* callback { nullptr };
};

inline JobCallback make_job_callback(FunctionObject& callback)
{
    return { &callback };
}

template<typename... Args>
[[nodiscard]] inline Value call_job_callback(VM& vm, JobCallback& job_callback, Value this_value, Args... args)
{
    VERIFY(job_callback.callback);
    auto& callback = *job_callback.callback;
    return vm.call(callback, this_value, args...);
}

}
