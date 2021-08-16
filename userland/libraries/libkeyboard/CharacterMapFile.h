/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/JsonObject.h>
#include <libkeyboard/CharacterMapData.h>

namespace Keyboard {

class CharacterMapFile {

public:
    static Optional<CharacterMapData> load_from_file(const String& filename);

private:
    static Vector<u32> read_map(const JsonObject& json, const String& name);
};

}
