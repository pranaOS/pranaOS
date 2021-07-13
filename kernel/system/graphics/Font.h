/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libtext/Rune.h>

int font_height();
int font_width();

void font_draw(Text::Rune cp, int x, int y);

void font_set_fg(int color);
void font_set_bg(int color);