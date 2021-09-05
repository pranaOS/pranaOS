/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <libkern/types.h>
#include <tasking/bits/dump.h>
#include <tasking/tasking.h>

int dump_impl(dump_data_t* data);
int dump_kernel_impl(dump_data_t* dump_data, const char* err_desc);
int dump_kernel_impl_from_tf(dump_data_t* dump_data, const char* err_desc, trapframe_t* tf);