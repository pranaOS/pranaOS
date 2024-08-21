/**
 * @file errno.h
 * @author Krisna Pranav
 * @brief errno
 * @version 6.0
 * @date 2023-07-05
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "errno_codes.h"
#include <sys/cdefs.h>

#define __RETURN_WITH_ERRNO(rc, good_ret, bad_ret) \
    do {                                           \
        if (rc < 0) {                              \
            errno = -rc;                           \
            return (bad_ret);                      \
        }                                          \
        errno = 0;                                 \
        return (good_ret);                         \
    } while (0)

__BEGIN_DECLS

extern const char* const sys_errlist[];

extern int sys_nerr;

#ifdef NO_TLS
extern int errno;
#else
extern __thread int errno;
#endif

#define errno errno

__END_DECLS