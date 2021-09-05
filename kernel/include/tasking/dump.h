/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <platform/generic/tasking/trapframe.h>
#include <tasking/proc.h>

void dump_and_kill(proc_t* p);
int dump_prepare_kernel_data();
int dump_kernel(const char* err);
int dump_kernel_from_tf(const char* err, trapframe_t* tf);