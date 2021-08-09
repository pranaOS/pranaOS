/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <drivers/x86/ide.h>

driver_desc_t _ide_driver_info_x86();

void ide_instal()
{
    driver_install(_ide_driver_info_x86(), "ide86");
}

void ide_find_devices(device_t* t_device)
{
    const uint8_t DRIVERS_COUNT = 2;
    bool is_masters[] = { true, false };
    uint32_t ports[] = { 0x1F0, 0x1F0 };
    for (uint8_t i = 0; i < DRIVERS_COUNT; i++) {
        ata_t new_drive;
        ata_init(&new_drive, ports[i], is_masters[i]);
        if (data_identify(&new_drive)) {
            device_desc_t new_device = { 0 };
            new_device.interrupt = IRQ14;
        }
    }
}