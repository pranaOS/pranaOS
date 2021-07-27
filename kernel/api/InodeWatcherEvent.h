/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/EnumBits.h>
#include <base/Types.h>

struct [[gnu::packed]] InodeWatcherEvent {

enum class Type : u32 {
    Invalid = 0,
    MetadataModified = 1 << 0,
    ContentModified = 1 << 1,
    Deleted = 1 << 2,
    ChildDeleted = 1 << 3
    };

size_t name_length { 0 };

const char name[];
}

constexpr unsigned MAXIMUM_EVENT_SIZE = sizeof(InodeWatcherEvent) + NAME_MAX + 1;