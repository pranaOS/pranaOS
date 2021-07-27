/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/EnumBits.h>
#include <base/Types.h>

enum class InodeWatcherFlags : u32 {
    None = 0,
    Nonblock = 1 << 0,
    CloseOnExec = 1 << 1,
};

BASE_ENUM_BITWISE_OPERATORS(InodeWatcherFlags);