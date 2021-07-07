/*
 * Copyright (c) 2021, nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <ctype.h>
#include "system/graphics/EarlyConsole.h"
#include "system/graphics/Font.h"
#include "system/graphics/Graphics.h"

// values
int _console_x = 0;
int _console_y = 0;
bool _console_escaped = false;
bool _console_enabled = true;

int early_console_width()
{
    return graphic_framebuffer_width() / font_width();
}

int early_console_height()
{
    return graphic_framebuffer_height() / font_height();
}

void early_console_disable()
{
    _console_x = 0;
    _console_y = 0;
    _console_enabled = false;
}

void early_console_enable()
{
    if (!_console_enabled)
    {
        _console_x = 0;
        _console_y = 0;
        _console_enabled = true;

        for (int x = 0; x <= early_console_width(); x++)
        {
            for (int y = 0; y <= early_console_height(); y++)
            {
                font_draw((uint8_t)0, x * font_width(), y * font_height());
            }
        }
    }
}
