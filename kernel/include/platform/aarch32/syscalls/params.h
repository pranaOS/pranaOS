/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_PLATFORM_AARCH32_SYSCALLS_PARAMS_H
#define _KERNEL_PLATFORM_AARCH32_SYSCALLS_PARAMS_H

// includes
#include <platform/aarch32/tasking/trapframe.h>

#define sys_id
#define param1
#define param2
#define param3
#define param4
#define param5
#define return_val
#define return_with_val(val) \
    (return_val = val);       \
    return


#endif