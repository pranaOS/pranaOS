/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "system/Streams.h"
#include <libmath/MinMax.h>
#include "bga/BGA.h"
#include "system/graphics/EarlyConsole.h"
#include "system/graphics/Graphics.h"
#include "system/handover/Handover.h"
#include "system/interrupts/Interupts.h"


void BGA::write_register(uint16_t address, uint16_t data)
{
    out16(BGA_ADDRESS, address);
    out16(BGA_DATA, data);
}

uint16_t BGA::read_register(uint16_t address)
{
    out16(BGA_ADDRESS, address);
    return in16(BGA_DATA);
}

JResult BGA::set_resolution(int width, int height)
{
    if (width * height * sizeof(uint32_t) > _framebuffer->size())
    {
        Kernel::logln("Not enoughs VRAM for setting the resolution to {}x{}.", width, height);

        return ERR_OUT_OF_MEMORY;
    }
    else
    {
        write_register(BGA_REG_ENABLE, BGA_DISABLED);
        write_register(BGA_REG_XRES, width);
        write_register(BGA_REG_YRES, height);
        write_register(BGA_REG_BPP, 32);
        write_register(BGA_REG_ENABLE, BGA_ENABLED | BGA_LINEAR_FRAMEBUFFER);

        if (read_register(BGA_REG_XRES) != width ||
            read_register(BGA_REG_YRES) != height)
        {
            write_register(BGA_REG_ENABLE, BGA_DISABLED);

            write_register(BGA_REG_XRES, _width);
            write_register(BGA_REG_YRES, _height);

            write_register(BGA_REG_BPP, 32);
            write_register(BGA_REG_ENABLE, BGA_ENABLED | BGA_LINEAR_FRAMEBUFFER);

            return ERR_INVALID_ARGUMENT;
        }

        _width = width;
        _height = height;

        graphic_did_find_framebuffer(
            _framebuffer->base(),
            _width,
            _height,
            _width * 4,
            4);

        Kernel::logln("Resolution set to {}x{}.", width, height);

        return SUCCESS;
    }
}

BGA::BGA(DeviceAddress address) : PCIDevice(address, DeviceClass::FRAMEBUFFER)
{
    _framebuffer = make<MMIORange>(bar(0).range());
    set_resolution(handover()->framebuffer_width, handover()->framebuffer_height);
}