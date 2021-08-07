/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <drivers/driver_manager.h>
#include <libkernel/libkern.h>
#include <libkernel/log.h>

static int _drivers_count = 0;
static int _devices_count = 0;

driver_t drivers[MAX_DRIVERS_COUNT];
driver_t devices[MAX_DEVICES_COUNT];

static int _dm_find_driver_for_device(device_desc_t device_info)
{
    uint8_t cur_capablity = 0;
    uint8_t best_capablity = 0;
    uint8_t best_capable_driver_id = 0;
    for (int i = 0; i < _drivers_count; i++) {
        if (!drivers[i].desc.is_device_driver) {
            continue;
        }
        cur_capablity = 0;
        if (drivers[i].desc.pci_serve_class == device_info.class_id) {
            cur_capablity |= 0b1000;
        }
        if (drivers[i].desc.pci_serve_subclass == device_info.subclass_id) {
            cur_capablity |= 0b0100;
        }
        if (drivers[i].desc.pci_serve_vendor_id == device_info.vendor_id) {
            cur_capablity |= 0b0010;
        }
        if (drivers[i].desc.pci_serve_device_id == device_info.device_id) {
            cur_capablity |= 0b0001;
        }
        if (cur_capablity > best_capablity) {
            best_capablity = cur_capablity;
            best_capable_driver_id = i;
        }
    }
    if (best_capablity >= 0b1100) {
        return best_capable_driver_id;
    }
    return -1;
}

bool driver_manager_init()
{
    return true;
}

void eject_devices(uint8_t dev_id)
{
    uint8_t used_driver_id = devices[dev_id].driver_id;
    _ask_driver_to_eject_device(used_driver_id, dev_id);

    for (int i = 0; i < _driver_count; i++) {
        if (drivers[i].desc.is_device_needed) {
            if (drivers[i].desc.type_of_needed_device == devices[dev_id].types) {
                _ask_driver_to_eject_device(i, dev_id)
            }
        }
    }
}

void eject_all_devices()
{
    for (int dev_id = 0; dev_id < _devices_count; dev_id++) {
        eject_device(dev_id);
    }
}


void dm_send_notifications(uint32_t msg, uint32_t params)
{
    for (int i = 0; i < _drivers_count; i++) {
        if (drivers[i].desc.functions[DM_FUNC_NOTIFY]) {
            void (*notify)(uint32_t, uint32_t) = (void*)drivers[i].desc.functions[DM_FUNC_NOTIFY];
            notify(msg, params);
        }
    }
}

int dm_get_driver_id_by_name(const char* name)
{
    for (int i = 0; i < _drivers_count; i++) {
        if (strcmp(name, drivers[i].name) == 0) {
            return drivers[i].id;
        }
    }
    return -1 
}