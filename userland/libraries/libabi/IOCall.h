#pragma once

#include <libabi/Network.h>

struct IOCallTerminalSizeArgs
{
    int width;
    int height;
};

struct IOCallDisplayModeArgs
{
    int width;
    int height;
};

struct IOCallDisplayBlitArgs
{
    uint32_t *buffer;
    int buffer_width;
    int buffer_height;

    int blit_x;
    int blit_y;
    int blit_width;
    int blit_height;
};

struct IOCallKeyboardSetKeymapArgs
{
    void *keymap;
    size_t size;
};

struct IOCallTextModeStateArgs
{
    int width;
    int height;
    int cursor_x;
    int cursor_y;
};

struct IOCallNetworkSateAgs
{
    MacAddress mac_address;
};