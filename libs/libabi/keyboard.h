/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <libtext/Rune.h>

class Keys {
public:
    static inline const char *key_to_string(Key key)
    {
        if (key < __KEY_COUNT)
        {
            return KEYS_NAMES[key];
        }
        else
        {
            return "overflow";
        }
    }

    static inline KeyMapping *keymap_lookup(KeyMap *keymap, Key key)
    {
        for (size_t i = 0; i < keymap->lenght; i++)
        {
            if (keymap->mappings[i].key == key)
            {
                return &keymap->mappings[i];
            }
        }
        return nullptr;
    }
}