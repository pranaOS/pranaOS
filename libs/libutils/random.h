/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libutils/platform.h>
#include <libutils/types.h>

#if defined(__pranaOS__)
#   include <stdlib.h>
#endif

#if defined(__unix__)
#   include <unistd.h>
#endif

#if defined(UTILS_OS_MACOS)
#   include <sys/random.h>
#endif