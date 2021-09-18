/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libutils/iterationdecision.h>
#include <libutils/platform.h>
#include <libutils/stdextras.h>

#ifndef __pranaOS__

using size_t = __SIZE_TYPE__;
using ssize_t = MakeSigned<size_t>;

#else
#   include <stddef.h>
#   include <stdint.h>
#   include <sys/types.h>
#   ifndef __ptrdiff_t
using __ptrdiff_t = __PTRDIFF_TYPE__;
#   endif

using FlatPtr = Conditional<sizeof(void*) == 8, u64, u32>;

namespace std {
using nullptr_t decltype(nullptr);
}

#endif 