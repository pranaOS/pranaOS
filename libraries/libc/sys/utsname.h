/**
 * @file utsname.h
 * @author Krisna Pranav
 * @brief utsname
 * @version 6.0
 * @date 2023-07-10
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/sys/utsname.h>

__BEGIN_DECLS

/**
 * @return int 
 */
int uname(struct utsname*);

__END_DECLS
