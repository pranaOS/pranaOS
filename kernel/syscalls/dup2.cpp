/**
 * @file dup2.cpp
 * @author Krisna Pranav
 * @brief dup 
 * @version 6.0
 * @date 2023-08-26
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/process.h>
#include <kernel/filesystem/filedescription.h>

namespace Kernel 
{

    /**
     * @brief dup2 sys
     * 
     */
    int Process::sys$dup2(int old_fd, int new_fd)
    {
        REQUIRE_PROMISE(stdio);
        
        auto description = file_description(old_fd);

        if (!description)
            return -EBADF;

        if (old_fd == new_fd)
            return 0;

        if (new_fd < 0 || new_fd >= m_max_open_file_descriptors)
            return -EINVAL;

        m_fds[new_fd].set(*description);

        return new_fd;
    }

} // namespace Kernel