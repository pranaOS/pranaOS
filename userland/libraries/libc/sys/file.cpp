/**
 * @file file.cpp
 * @author Krisna Pranav
 * @brief file
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#include <mods/assertions.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/file.h>

extern "C" {

    /**
     * @param fd 
     * @param operation 
     * @return int 
     */
    int flock(int fd, int operation)
    {
        struct flock lock {
            short(operation & 0b11), SEEK_SET, 0, 0, 0
        };

        if (operation & LOCK_NB) {
            return fcntl(fd, F_SETLK, &lock);
        }

        VERIFY_NOT_REACHED();
    }
}
