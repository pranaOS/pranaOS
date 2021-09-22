/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <libkern/c_attrs.h>
#include <libkern/log.h>
#include <libkern/types.h>
#include <platform/generic/system.h>
#include <platform/generic/tasking/trapframe.h>

#define ASSERT(x)                                              \
    if (unlikely(!(x))) {                                      \
        log("kassert at line %d in %s\n", __LINE__, __FILE__); \
        system_stop();                                         \
    }

void kpanic(const char* msg) NORETURN;
void kpanic_tf(const char* err_msg, trapframe_t* tf) NORETURN;