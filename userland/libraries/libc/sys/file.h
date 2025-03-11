/**
 * @file file.h
 * @author Krisna Pranav
 * @brief file
 * @version 6.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>

__BEGIN_DECLS

#define LOCK_SH 0
#define LOCK_EX 1
#define LOCK_UN 2
#define LOCK_NB (1 << 2)

/**
 * @param fd 
 * @param operation 
 * @return int 
 */
int flock(int fd, int operation);

__END_DECLS
