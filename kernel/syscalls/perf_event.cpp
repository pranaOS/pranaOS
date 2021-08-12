/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/performanceEventBuffer.h>
#include <kernel/Process.h>

namespace Kernel {

KResultOr<FlatPtr> Process::sys$perf_event(int type, FlatPtr arg1, FlatPtr arg2)
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this)
    auto* events_buffer = current_perf_events_buffer();
    if (!events_buffer)
        return KSuccess;
    return events_buffer->append(type, arg1, arg2, nullptr);
}

KResultOr<FlatPtr> Process::sys$perf_register_string(Userspace<char const*> user_string, size_t user_string_length)
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this)
    auto* events_buffer = current_perf_events_buffer();
    if (!events_buffer)
        return KSuccess;

    auto string_or_error = try_copy_kstring_from_user(user_string, user_string_length);
    if (string_or_error.is_error())
        return string_or_error.error();

    return events_buffer->register_string(string_or_error.release_value());
}

}
