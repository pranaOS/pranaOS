/*
* Copyright (c) 2021, Krisna Pranav
*
* SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <libabi/process.h>
#include <libutils/macros.h>

typedef uint64_t size64_t;
typedef int64_t ssize64_t;

#ifndef __cplusplus
#   ifndef NO_PRANAOS_USING_UTILS_NAMESPACE

namespace Utils
{
}

#   endif
#endif