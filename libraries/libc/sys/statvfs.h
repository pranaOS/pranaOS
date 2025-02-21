/**
 * @file statvfs.h
 * @author Krisna Pranav
 * @brief statvfs
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/sys/statvfs.h>

__BEGIN_DECLS

/**
 * @param path 
 * @param buf 
 * @return int 
 */
int statvfs(char const* path, struct statvfs* buf);

/**
 * @param fd 
 * @param buf 
 * @return int 
 */
int fstatvfs(int fd, struct statvfs* buf);

__END_DECLS
