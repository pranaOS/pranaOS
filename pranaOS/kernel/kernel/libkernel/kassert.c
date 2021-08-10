/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libkernel/kassert.h>
#include <tasking/dump.h>

void kpanic(const char* err_msg)
{
    dump_kernel(err_msg);
    system_stop();
}

void kpanic_tf(const char* err_msg, trapframe_t* tf)
{
    dump_kernel_from_tf(err_msg, tf);
    system_stop();
}