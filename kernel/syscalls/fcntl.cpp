/**
 * @file fcntl.cpp
 * @author Krisna Pranav
 * @brief fcntl
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
     * @brief fcntl
     * 
     */
    int Process::sys$fcntl(int fd, int cmd, u32 arg)
    {
        REQUIRE_PROMISE(stdio);
    #ifdef DEBUG_IO
        dbg() << "sys$fcntl: fd=" << fd << ", cmd=" << cmd << ", arg=" << arg;
    #endif
        auto description = file_description(fd);
        if (!description)
            return -EBADF;

        switch (cmd) {
        case F_DUPFD: {
            int arg_fd = (int)arg;
            if (arg_fd < 0)
                return -EINVAL;
            int new_fd = alloc_fd(arg_fd);
            if (new_fd < 0)
                return new_fd;
            m_fds[new_fd].set(*description);
            return new_fd;
        }
        
        case F_GETFD:
            return m_fds[fd].flags();
        case F_SETFD:
            m_fds[fd].set_flags(arg);
            break;
        case F_GETFL:
            return description->file_flags();
        case F_SETFL:
            description->set_file_flags(arg);
            break;
        case F_ISTTY:
            return description->is_tty();
        default:
            return -EINVAL;
        }
        return 0;
    }

} // namespace Kernel
