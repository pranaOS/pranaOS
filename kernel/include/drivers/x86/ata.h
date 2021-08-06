/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_DRIVERS_X86_ATA_H
#define _KERNEL_DRIVERS_X86_ATA_H

// includes
#include <drivers/driver_manager.h>
#include <drivers/x86/display.h>
#include <libkernel/types.h>
#include <mem/kmalloc.h>
#include <platform/x86/port.h>

typedef struct { 
    uint32_t data; 
    uint32_t error;
    uint32_t sector_count; 
    uint32_t lba_lo;
    uint32_t lba_mid; 
    uint32_t lba_hi;
    uint32_t device; 
    uint32_t command; 
    uint32_t control;
} ata_ports_t;

typedef struct {
    ata_ports_t port;
    bool is_master;
    uint16_t cylindres;
    uint16_t heads;
    uint16_t sectors;
    bool dma;
    bool lba;
    uint32_t capacity; 
} ata_t;

extern ata_t _ata_drives[MAX_DEVICES_COUNT];

void ata_add_new_device(device_t* t_new_device);

void ata_install();
void ata_init(ata_t* ata, uint32_t port, bool is_master);
bool ata_indentify(ata_t* ata);

#endif 