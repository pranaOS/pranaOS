/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libsystem/io/Stream.h>
#include "system/Configs.h"
#include "system/Streams.h"
#include "ps2/LegacyKeyboard.h"

KeyMap *keyboard_load_keymap(const char *keymap_path)
{
    CLEANUP(stream_cleanup)
    Stream *keymap_file = stream_open(keymap_path, J_OPEN_READ);

    if (handle_has_error(keymap_file))
    {
        Kernel::logln("Failed to load keymap from {}: {}", keymap_path, handle_error_string(keymap_file));
        return nullptr;
    }

    JStat stat;
    stream_stat(keymap_file, &stat);

    if (stat.type != J_FILE_TYPE_REGULAR)
    {
        Kernel::logln("Failed to load keymap from {}: This is not a regular file", keymap_path);

        return nullptr;
    }

    Kernel::logln("Allocating keymap of size {}kio", stat.size / 1024);
    KeyMap *keymap = (KeyMap *)malloc(stat.size);

    size_t read = stream_read(keymap_file, keymap, stat.size);

    if (read != stat.size)
    {
        Kernel::logln("Failed to load keymap from {}: {}", keymap_path, handle_error_string(keymap_file));

        free(keymap);

        return nullptr;
    }

    return keymap;
}