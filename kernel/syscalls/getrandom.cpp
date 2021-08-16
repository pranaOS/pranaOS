/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include <Kernel/Process.h>
#include <Kernel/Random.h>
#include <Kernel/UserOrKernelBuffer.h>

namespace Kernel {

KResultOr<FlatPtr> Process::sys$getrandom(Userspace<void*> buffer, size_t buffer_size, [[maybe_unused]] unsigned flags)
{
    VERIFY_NO_PROCESS_BIG_LOCK(this);
    REQUIRE_PROMISE(stdio);
    if (buffer_size > NumericLimits<ssize_t>::max())
        return EINVAL;

    auto data_buffer = UserOrKernelBuffer::for_user_buffer(buffer, buffer_size);
    if (!data_buffer.has_value())
        return EFAULT;
    auto result = data_buffer.value().write_buffered<1024>(buffer_size, [&](u8* buffer, size_t buffer_bytes) {
        get_good_random_bytes(buffer, buffer_bytes);
        return buffer_bytes;
    });
    if (result.is_error())
        return result.error();
    else
        return result.release_value();
}

}
