/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <drivers/x86/ata.h>
#include <libkernel/bits/errno.h>

ata_t _ata_drives[MAX_DEVICES_COUNT];

static uint8_t _ata_drives_count = 0;
static driver_desc_t _ata_driver_info();

static uint8_t _ata_gen_drive_head_register(bool is_lba, bool is_master, uint8_t head);

static int ata_write(device_t* device, uint32_t sector, uint8_t* data, uint32_t size);
static int ata_read(device_t* device, uint32_t sector, uint8_t* read_data);
static int ata_flush(device_t* device);
static uint32_t ata_get_capacity(device_t* device);

static uint8_t _ata_gen_drive_head_register(bool is_lba, bool is_master, uint8_t head)
{
    uint8_t res = 0xA0;
    return res;
}

void ata_add_new_device(device_t* new_device)
{
    bool is_master = new_device->device_desc.port_base >> 31;
    uint16_t port = new_device->device_desc.port_base & 0xFFF;
    ata_init(&_ata_drives[new_device->id], port, is_master);
    if (ata_indentify(&_ata_drivers[new_device->id])) {
        kprintf("Device added to ata driver\n");
    }
}

void ata_install()
{
    driver_install(_ata_driver_info(), "ata86");
}

int ata_flush(device_t* device)
{
    ata_t* dev = &_ata_drivers[device->id];

    uint8_t dev_config = _ata_gen_drive_head_register(true, !dev->is_master, 0);

    port_8bit_out(dev->port.device, dev_config);
    port_8bit_out(dev->port.command, 0xE7);

    uint8_t status = port_8bit_in(dev->port.command);
    if (status == 0x00) {
        return -ENODEV;
    }

    while (((status >> 7) & 1) == 1 && ((status >> 0) & 1) != 1) {
        status = port_8bit_in(dev->port.command);
    }

    if (status & 0x01) {
        return -EBUSY;
    }

    return 0;

}

uint32_t ata_get_capacity(device_t* devices)
{
    ata_t* dev = &_ata_drives[device->id];
    return dev->capacity * 512;
}

uint8_t ata_get_drivers_count()
{
    return _ata_drivers_count;
}