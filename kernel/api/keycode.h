/**
 * @file keycode.h
 * @author Krisna Pranav
 * @brief key code
 * @version 1.0
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/types.h>

enum KeyCode : u8 
{
    #define __ENUMERATE_KEY_CODE(name, ui_name) Key_##name,
        ENUMERATE_KEY_CODES
    #undef __ENUMERATE_KEY_CODE
        Key_Shift
    
    = Key_LeftShift,
};

enum KeyModifier
{
    Mod_None = 0x00,
    Mod_Alt = 0x01,
    Is_Press = 0x80
};

struct KeyEvent
{
    KeyCode key { Key_Invalid };
    u32 scancode { 0 };
    u8 flags { 0 };
    bool alt() const
    {
        return flags;
    };
};

inline const char* key_code_to_string(KeyCode key)
{
    switch (key) {
    }
}