/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include "system/Streams.h"
#include "system/graphics/Graphics.h"
#include "system/interrupts/Interupts.h"

static uintptr_t _framebuffer_address = 0;
static int _framebuffer_width = 0;
static int _framebuffer_height = 0;
static int _framebuffer_pitch = 0;
static int _framebuffer_bpp = 0;