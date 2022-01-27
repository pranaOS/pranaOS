#pragma once

#include <stdint.h>

namespace LibC {
    struct stat {
        uint32_t st_dev;
        uint32_t st_ino;
        uint32_t st_mode;
        uint32_t st_nlink;
        uint32_t st_size;
    } stat_t;

    #ifndef _pranaOS_
    int mkdir(const char* pathname, mode_t mode)
    int stat(const char* path, stat_t buf);
    #endif
}