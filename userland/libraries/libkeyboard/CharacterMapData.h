/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Types.h>

#define CHAR_MAP_SIZE 0x80

namespace Keyboard {

struct CharacterMapData {
    u32 map[CHAR_MAP_SIZE];
    u32 shift_map[CHAR_MAP_SIZE];
    u32 alt_map[CHAR_MAP_SIZE];
    u32 altgr_map[CHAR_MAP_SIZE];
    u32 shift_altgr_map[CHAR_MAP_SIZE];
};

}
