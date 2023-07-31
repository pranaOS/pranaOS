/**
 * @file ioctl.h
 * @author Krisna Pranav
 * @brief ioctl
 * @version 1.0
 * @date 2023-07-31
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <sys/cdefs.h>
#include <sys/ioctl_numbers.h>

__BEGIN_DECLS

/**
 * @param fd 
 * @param request 
 * @param ... 
 * @return int 
 */
int ioctl(int fd, unsigned request, ...);

__END_DECLS
