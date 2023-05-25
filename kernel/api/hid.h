/**
 * @file hid.h
 * @author Krisna Pranav
 * @brief HID
 * @version 0.1
 * @date 2023-05-25
 * 
 * @copyright Copyright (c) 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "types.h"

__DECL_BEGIN

struct mouse_event {
    int x;
    int y;
    int z;
    uint8_t buttons;
    bool absolute;
};

struct keyboard_event {
    uint16_t scancode;
    uint8_t key;
    uint8_t character;
    uint8_t modifiers;
};

#ifdef __cplusplus 
typedef struct mouse_event MouseEvent;
typedef struct keyboard_event KeyboardEvent;
#endif

__DECL_END