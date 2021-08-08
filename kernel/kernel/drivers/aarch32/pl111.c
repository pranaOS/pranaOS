/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <drivers/aarch32/pl111.h>
#include <fs/devfs/devfs.h>
#include <fs/vfs.h>
#include <libkernel/bits/errno.h>
#include <libkernel/libkernel.h>
#include <libkernel/log.h>
#include <mem/vmm/vmm.h>
#include <mem/vmm/zoner.h>
#include <tasking/tasking.h>

#define DEBUG_PL111

static zone_t mapped_zone;
static volatile pl111_registers_t* registers = (pl111_registers_t*)PL111_BASE;
static char* pl111_bufs_paddr[2];
static uint32_t pl111_screen_width;
static uint32_t pl111_screen_height;
static uint32_t pl111_screen_buffer_size;

static inline int _pl111_map_itself()
{
    mapped_zone = zoner_new_zone(sizeof(pl111_registers_t));
    vmm_map_page(mapped_zone.start, PL111_BASE, PAGE_READABLE | PAGE_WRITABLE | PAGE_EXECUTABLE);
    registers = (pl111_registers_t*)mapped_zone.ptr;
    return 0;
}

void pl111_init(device_t* dev)
{
#ifdef DEBUG_PL111
    log("PL111: Turning on");
#endif
#ifdef TARGET_DESKTOP
    _pl111_init_buffer(1024, 768);
    pl111_set_resolution(1024, 768);
#elif TARGET_MOBILE
    _pl111_init_buffer(320, 568);
    pl111_set_resolution(320, 568);
#endif

    volatile uint32_t ctl = registers->lcd_control;
    ctl &= ~LCD_POWER_MASK
            | ~LCD_BGR_MASK
            | ~LCD_BPP_MASK
            | ~LCD_EN_MASK;

    ctl |= LCD_POWER_MASK
            | LCD_BGR_MASK
            | LCD_TFT_MASK
            | (LCD_24_BPP << LCD_BPP_POS)
            | LCD_EN_MASK;

    registers->lcd_control = ctl;
}

static driver_desc_t _pl111_driver_info()
{
    driver_desc_t pl111_desc = { 0 };
    pl111_desc.type = DRIVER_VIDEO_DEVICE;
    pl111_desc.auto_start = true;
    pl111_desc.is_device_driver = false;
    pl111_desc.is_device_needed = false;
    pl111_desc.is_driver_needed = false;
    pl111_desc.functions[DRIVER_NOTIFICATION] = pl111_recieve_notification;
    pl111_desc.functions[DRIVER_VIDEO_INIT] = pl111_init;
    pl111_desc.functions[DRIVER_VIDEO_SET_RESOLUTION] = pl111_set_resolution;
    pl111_desc.pci_serve_class = 0x00;
    pl111_desc.pci_serve_subclass = 0x00;
    pl111_desc.pci_serve_vendor_id = 0x1234;
    pl111_desc.pci_serve_device_id = 0x1111;
    return pl111_desc;
}

void pl111_install()
{
    if (_pl111_map_itself()) {
#ifdef DEBUG_PL111
        log_error("PL111: Can't map itself!");
#endif
        return;
    }

    driver_install(_pl111_driver_info(), "pl111");
}