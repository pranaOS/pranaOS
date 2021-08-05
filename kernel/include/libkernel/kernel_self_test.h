/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_LIBKERNEL_KERNEL_SELF_TEST_H
#define _KERNEL_LIBKERNEL_KERNEL_SELF_TEST_H

#include <libkernel/types.h>

void kpanic_at_test(char* t_err_msg, uint16_t test_no);
bool kernel_self_test(bool throw_kernel_panic);

#endif 