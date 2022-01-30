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
    TERM_COLOR_WHITE,
    TERM_COLOR_CYAN
} term_color_t;
