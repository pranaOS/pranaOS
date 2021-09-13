#pragma once

#include <libabi/rune.h>

#define KEY_ENUM_ENTRY(__key_name, __key_number) __key_name,

enum Key: uint32_t
{
    KEY_LIST(KEY_ENUM_ENTRY)
        __KEY_COUNT,
};

enum KeyMotion : uint32_t
{
    KEY_MOTION_UP,
    KEY_MOTION_DOWN,
    KEY_MOTION_TYPED,
};

struct KeyMap
{
    char magic[4];
    char language[KEYMAP_LANGUAGE_SIZE];
    char region[KEYMAP_REGION_SIZE];

    uint32_t length;
    KeyMapping mappings[];
};

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
    for (size_t i = 0; i < keymap->length; i++)
    {
        if (keymap->mappings[i].key == key)
        {
            return &keymap->mappings[i];
        }
    }

    return null;
}