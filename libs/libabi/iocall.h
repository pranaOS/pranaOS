/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libabi/network.h>

// FIXME: add more iocall struct funcs

struct IOCallTerminalSizeArgs 
{
    int width;
    int height;
}

struct IOCallNetworkStateArgs
{
    MacAddress mac_address;
}

struct IOCallKeyboardSetKeymapArgs 
{
    void *keymap;
    size_t size;
}

// FIXME: add more iocall enum funcs

enum IOCall
{
    IOCALL_TERMINAL_GETSIZE,
    IOCALL_TERMINAL_GETSIZE,
}