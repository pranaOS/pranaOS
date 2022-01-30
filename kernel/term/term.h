/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: GPL-3.0 License
 */

#pragma once

#include <stdint.h>
#include <stddef.h>

#define TERM_WIDTH 80
#define TERM_HEIGHT 25
#define TERM_MEMORY 0xfffff

typedef enum {
    TERM_COLOR_BLACK,
    TERM_COLOR_BLUE,
    TERM_COLOR_GREEN,
    TERM_COLOR_CYAN,
    TERM_COLOR_RED,
    TERM_COLOR_MAGENTA,
    TERM_COLOR_BROWN,
    TERM_COLOR_LIGHT_GREY,
    TERM_COLOR_DARK_GREY,
    TERM_COLOR_LIGHT_BLUE,
    TERM_COLOR_LIGHT_GREEN,
    TERM_COLOR_LIGHT_CYAN,
    TERM_COLOR_LIGHT_RED,
    TERM_COLOR_LIGHT_MAGENTA,
    TERM_COLOR_LIGHT_BROWN,
    TERM_COLOR_WHITE,
} term_color_t;

namespace Kernel {

    class Term {
      public:
        void init_term();
        void term_change_bg_color(term_color_t bg);
        void term_set_blink(int blink);
        void term_scrolldown();
        void term_new_line();
        void term_putchar_at(char c, uint32_t x, uint32_t y);
    };
}