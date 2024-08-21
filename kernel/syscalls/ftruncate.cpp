/**
 * @file ftruncate.cpp
 * @author Krisna Pranav
 * @brief ftruncate
 * @version 6.0
 * @date 2023-08-26
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/filesystem/filedescription.h>
#include <kernel/process.h>

namespace Kernel 
{
    /**
     * @brief ftruncate sys in process
     * 
     */
    int Process::sys$ftruncate(int fd, off_t length)
    {
        REQUIRE_PROMISE(stdio);

        if (length < 0)
            return -EINVAL;
            
        auto description = file_description(fd);

        if (!description)
            return -EBADF;
            
        if (!description->is_writable())
            return -EBADF;

        return description->truncate(static_cast<u64>(length));
    } // int

} // namespace Kernel
