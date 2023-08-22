/**
 * @file chown.cpp
 * @author Krisna Pranav
 * @brief chown
 * @version 6.0
 * @date 2023-08-22
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/process.h>
#include <kernel/filesystem/filedescription.h>

namespace Kernel
{  
    /**
     * @brief fchown sys
     * 
     */
    int Process::sys$fchown(int fd, uid_t uid, gid_t gid)
    {
        REQUIRE_PROMISE(chown);

        auto description = file_description(fd);

        if (!description)   
            return -EBADF;
        
        return description->chown(uid, gid);
    }

} // namespace Kernel