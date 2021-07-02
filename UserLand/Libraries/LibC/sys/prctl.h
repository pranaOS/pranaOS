/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <sys/cdefs.h>
#include <sys/prctl_numbers.h>
#include <sys/types.h>

__BEGIN_DECLS

int prctl(int option, uintptr_t arg1, uintptr_t arg2);

__END_DECLS