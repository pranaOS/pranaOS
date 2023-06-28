/**
 * @file types.h
 * @author Krisna Pranav
 * @brief Types
 * @version 1.0
 * @date 2023-06-28
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "iterdecision.h"
#include "platform.h"
#include "stdlibextra.h"

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
using ssize_t = MakeSigned<size_t>::Type;

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

using pid_t = int;

#else
#    include <stddef.h>
#    include <stdint.h>
#    include <sys/types.h>

#    ifdef __ptrdiff_t
using __ptrdiff_t = __PTRDIFF_TYPE__;
#    endif

#endif

/**
 * @brief Conditional
 * 
 */
using FlatPtr = Conditional<sizeof(void*) == 8, u64, u32>::Type;

constexpr unsigned KiB = 1024;
constexpr unsigned MiB = KiB * KiB;
constexpr unsigned GiB = KiB * KiB * KiB;

namespace std {
    using nullptr_t = decltype(nullptr);
}

/**
 * @brief explode_byte
 * 
 * @param b 
 * @return constexpr u32 
 */
static constexpr u32 explode_byte(u8 b) {
    return b << 24 | b << 16 | b << 8 | b;
}

/**
 * @brief static_assert(explode_byte)
 * 
 */
static_assert(explode_byte(0xff) == 0xffffffff);
static_assert(explode_byte(0x80) == 0x80808080);
static_assert(explode_byte(0x7f) == 0x7f7f7f7f);
static_assert(explode_byte(0) == 0);

/**
 * @brief align_up_to
 * 
 * @param value 
 * @param alignment 
 * @return constexpr size_t 
 */
constexpr size_t align_up_to(const size_t value, const size_t alignment) {
    return (value + (alignment - 1)) & ~(alignment - 1);
}

/**
 * @brief TriState
 * 
 */
enum class [[nodiscard]] TriState : u8 {
    False,
    True,
    Unknown
};
