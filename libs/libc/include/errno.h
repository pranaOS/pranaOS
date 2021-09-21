/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <bits/errno.h>

extern int errno;

#define set_errno(x) (errno = x)