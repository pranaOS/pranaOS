/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "system/Streams.h"
#include "system/graphics/Graphics.h"
#include "system/interrupts/Interupts.h"

static uintptr_t _framebuffer_address = 0;
static int _framebuffer_width = 0;
static int _framebuffer_height = 0;
static int _framebuffer_pitch = 0;
static int _framebuffer_bpp = 0;

void graphic_early_initialize(Handover *handover)
{
    graphic_did_find_framebuffer(
        handover->framebuffer_addr,
        handover->framebuffer_width,
        handover->framebuffer_height,
        handover->framebuffer_pitch,
        handover->framebuffer_bpp);
}

void graphic_initialize(Handover *handover)
{
    if (_framebuffer_address != 0)
    {
        return;
    }

    framebuffer_initialize(handover);
}


void graphic_did_find_framebuffer(uintptr_t address, int width, int height, int pitch, int bpp)
{
    InterruptsRetainer retainer;

    _framebuffer_address = address;
    _framebuffer_width = width;
    _framebuffer_height = height;
    _framebuffer_pitch = pitch;
    _framebuffer_bpp = bpp;

    Kernel::logln("Framebuffer: w:{} h:{} p:{} bpp:{}", width, height, pitch, bpp);
}


bool graphic_has_framebuffer()
{
    return _framebuffer_address != 0;
}

uint32_t *graphic_framebuffer()
{
    return reinterpret_cast<uint32_t *>(_framebuffer_address);
}

int graphic_framebuffer_width()
{
    return _framebuffer_width;
}

int graphic_framebuffer_pitch()
{
    return _framebuffer_pitch;
}