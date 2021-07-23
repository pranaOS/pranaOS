/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

#include <base/Types.h>

namespace Base::SIMD {

using i8x2 = i8 __attribute__((vector_size(2)));
using i8x4 = i8 __attribute__((vector_size(4)));
using i8x8 = i8 __attribute__((vector_size(8)));
using i8x16 = i8 __attribute__((vector_size(16)));
using i8x32 = i8 __attribute__((vector_size(32)));

using i16x2 = i16 __attribute__((vector_size(4)));
using i16x4 = i16 __attribute__((vector_size(8)));
using i16x8 = i16 __attribute__((vector_size(16)));
using i16x16 = i16 __attribute__((vector_size(32)));

using c8x2 = char __attribute__((vector_size(2)));
using c8x4 = char __attribute__((vector_size(4)));
using c8x8 = char __attribute__((vector_size(8)));
using c8x16 = char __attribute__((vector_size(16)));
using c8x32 = char __attribute__((vector_size(32)));

using i32x2 = i32 __attribute__((vector_size(8)));
using i32x4 = i32 __attribute__((vector_size(16)));
using i32x8 = i32 __attribute__((vector_size(32)));

using i64x2 = i64 __attribute__((vector_size(16)));
using i64x4 = i64 __attribute__((vector_size(32)));
}