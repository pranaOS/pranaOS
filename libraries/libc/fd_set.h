/**
 * @file fd_set.h
 * @author Krisna Pranav
 * @brief fd_set
 * @version 6.0
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#define FD_SETSIZE 1024
#define FD_ZERO(set)                      \
    do {                                  \
        memset((set), 0, sizeof(fd_set)); \
    } while (0)
#define FD_CLR(fd, set) ((set)->fds_bits[(fd / 8)] &= ~(1 << (fd) % 8))
#define FD_SET(fd, set) ((set)->fds_bits[(fd / 8)] |= (1 << (fd) % 8))
#define FD_ISSET(fd, set) ((set)->fds_bits[(fd / 8)] & (1 << (fd) % 8))

struct __fd_set {
    unsigned char fds_bits[FD_SETSIZE / 8];
};

typedef struct __fd_set fd_set;
