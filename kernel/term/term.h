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
#define TERM_MEMORY 0xB8000

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

        /**
         * @param bg, fg 
         */
        void term_change_bg_color(term_color_t bg);
        void term_set_blink(int blink);
        void term_scrolldown();
        void term_new_line();
        void term_putchar_at(char c, uint32_t x, uint32_t y);
        void term_putchar(char c);
        void term_write_string(const uint8_t* data);
        int term_interpret_ansi(char c);

        /**
         * @return uint32_t 
         */
        uint32_t term_get_row();
        uint32_t term_get_column();
        uint8_t term_get_color();
        uint8_t term_get_fg_color();
        uint8_t term_get_bg_color();
        uint16_t* term_get_buffer();

        /**
         * @param row 
         */
        void term_set_row(uint32_t row);
        void term_set_column(uint32_t column);
        void term_set_color(uint8_t color);
        void term_set_fg_color(term_color_t color);
        void term_set_bg_color(term_color_t color);
        void term_set_buffer(uint16_t* buffer);

        /**
         * @brief initializing function
         * 
         * @return true 
         * @return false 
         */
        bool initialize(); 

      private:
        Term();

        /**
         * @brief Get the fg color object
         * 
         * @param bg 
         */
        void get_fg_color(term_color_t bg);
        void get_bg_color(term_color_t fg);
    };
}

