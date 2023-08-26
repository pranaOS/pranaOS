/**
 * @file get_dir_entries.cpp
 * @author Krisna Pranav
 * @brief get directory entries
 * @version 6.0
 * @date 2023-08-26
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/process.h>
#include <kernel/filesystem/filedescription.h>

namespace Kernel 
{
    /**
     * @brief get directory entries process
     * 
     */
    ssize_t Process::sys$get_dir_entries(int fd, void* buffer, ssize_t size)
    {
        REQUIRE_PROMISE(stdio);

        if (size < 0)
            return -EINVAL;

        auto description = file_description(fd);

        if (!description)
            return -EBADF;

        auto user_buffer = UserOrKernelBuffer::for_user_buffer((u8*)buffer, size);
        
        if (!user_buffer.has_value())
            return -EFAULT;

        return description->get_dir_entries(user_buffer.value(), size);
    }

} // namespace Kernel
