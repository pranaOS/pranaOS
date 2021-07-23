/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/IterationDecision.h>
#include <base/Platform.h>
#include <base/StdLibExtras.h>

using u64 = __UINT64_TYPE__;
using u32 = __UINT32_TYPE__;
using u16 = __UINT16_TYPE__;
using u8 = __UINT8_TYPE__;
using i64 = __INT64_TYPE__;
using i32 = __INT32_TYPE__;
using i16 = __INT16_TYPE__;
using i8 = __INT8_TYPE__;

#ifdef __pranaos__

using size_t = __SIZE_TYPE__;
using ssize_t = MakeSigned<size_t>;

using ptrdiff_t = __PTRDIFF_TYPE__;

using intptr_t = __INTPTR_TYPE__;
using uintptr_t = __UINTPTR_TYPE__;

using uint8_t = u8;
using uint16_t = u16;
using uint32_t = u32;
using uint64_t = u64;

using int8_t = i8;
using int16_t = i16;
using int32_t = i32;
using int64_t = i64;