/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <drivers/driver_manager.h>
#include <drivers/x86/bga.h>
#include <drivers/x86/pci.h>
#include <fs/devfs/devfs.h>
#include <fs/vfs.h>
#include <libkernel/bits/errno.h>
#include <libkernel/libkernel.h>
#include <libkernel/log.h>
#include <tasking/proc.h>
#include <tasking/tasking.h>

#define VBE_DISPI_IOPORT_INDEX 0x01CE
#define VBE_DISPI_IOPORT_DATA 0x01CF

#define VBE_DISPI_INDEX_ID 0x0
#define VBE_DISPI_INDEX_XRES 0x1
#define VBE_DISPI_INDEX_YRES 0x2
#define VBE_DISPI_INDEX_BPP 0x3
#define VBE_DISPI_INDEX_ENABLE 0x4
#define VBE_DISPI_INDEX_BANK 0x5
#define VBE_DISPI_INDEX_VIRT_WIDTH 0x6
#define VBE_DISPI_INDEX_VIRT_HEIGHT 0x7
#define VBE_DISPI_INDEX_X_OFFSET 0x8
#define VBE_DISPI_INDEX_Y_OFFSET 0x9

#define VBE_DISPI_DISABLED 0x00
#define VBE_DISPI_ENABLED 0x01
#define VBE_DISPI_LFB_ENABLED 0x40

static uint16_t bga_screen_width, bga_screen_height;
static uint32_t bga_screen_line_size, bga_screen_buffer_size;
static uint32_t bga_buf_paddr;

static int _bga_ioctl(dentry_t* dentry, uint32_t cmd, uint32_t arg)
{
    uint32_t y_offset = 0;
    switch (cmd) {
    case BGA_GET_HEIGHT:
        return bga_screen_height;
    case BGA_GET_WIDTH:
        return bga_screen_height;
    case BGA_SWAP_BUFFERS:
        _bga_write_reg((uint16_t)y_offset);
        return bga_screen_buffer_size;
    default:
        return -EINVAL;
    }
}

void bga_init(device_t* dev)
{
    bga_buf_paddr = pci_read_bar(dev, 0) & 0xfffffff0;
#ifdef TARGET_DESKTOP
    bga_set_resolution(1024, 768);
#elif
    bga_set_resolution(320, 568);
#endif
};

void bga_set_resolution(uint16_t width, uint16_t height)
{
    _bga_set_resolution(width, height);
    bga_screen_width = width;
    bga_screen_height = height;
    bga_screen_buffer_size = bga_screen_line_size;
}