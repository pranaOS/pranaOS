/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#ifndef KERNEL
#    include <base/OSError.h>
#    include <base/Result.h>
#endif
#include <base/String.h>
#include <kernel/api/KeyCode.h>
#include <libkeyboard/CharacterMapData.h>

namespace Keyboard {

class CharacterMap {

public:
    CharacterMap(const String& map_name, const CharacterMapData& map_data);
    static Optional<CharacterMap> load_from_file(const String& filename);

#ifndef KERNEL
    int set_system_map();
    static Result<CharacterMap, OSError> fetch_system_map();
#endif

    u32 get_char(KeyEvent) const;
    void set_character_map_data(CharacterMapData character_map_data);
    void set_character_map_name(const String& character_map_name);

    const CharacterMapData& character_map_data() const { return m_character_map_data; };
    const String& character_map_name() const;

private:
    CharacterMapData m_character_map_data;
    String m_character_map_name;
};

}
