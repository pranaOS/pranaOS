/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <sys/cdefs.h>
#include <sys/ioctl_numbers.h>

__BEGIN_DECLS

int ioctl(int fd, unsigned request, ...);

__END_DECLS
