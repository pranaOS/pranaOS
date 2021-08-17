/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _KERNEL_LIBKERN_KASSERT_H
#define _KERNEL_LIBKERN_KASSERT_H

#include <libkernel/c_attrs.h>
#include <libkernel/log.h>
#include <libkernel/types.h>
#include <platform/generic/system.h>
#include <platform/generic/tasking/trapframe.h>

#define ASSERT(x)                                              \
    if (unlikely(!(x))) {                                      \
        log("kassert at line %d in %s\n", __LINE__, __FILE__); \
        system_stop();                                         \
    }

void kpanic(const char* msg) NORETURN;
void kpanic_tf(const char* err_msg, trapframe_t* tf) NORETURN;

#endif
