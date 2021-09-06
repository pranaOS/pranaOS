/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <platform/x86/tasking/trapframe.h>

#define SYSCALL_HANDLER_NO 0x80

#define sys_id (tf->eax)
#define param1 (tf->ebx)
#define param2 (tf->ecx)
#define param3 (tf->edx)
#define param4 (tf->esi)
#define param5 (tf->edi)
#define return_val (tf->eax)
#define return_with_val(val) \
    (return_val = val);      \
    return
