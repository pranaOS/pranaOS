/**
 * @file prctl.h
 * @author Krisna Pranav
 * @brief prctl
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>
#include <sys/prctl_numbers.h>
#include <sys/types.h>

__BEGIN_DECLS

/**
 * @param option 
 * @param ... 
 * @return int 
 */
int prctl(int option, ...);

__END_DECLS
