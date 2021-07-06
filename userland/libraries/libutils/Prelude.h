/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <libabi/Process.h>
#include <libutils/Macros.h>

typedef uint64_t size64_t;
typedef int64_t ssize64_t;

#ifdef __cplusplus
#    ifndef NO_PRANAOS_USING_UTILS_NAMESPACE

namespace Utils
{
}

using namespace Utils;

#    endif
#endif