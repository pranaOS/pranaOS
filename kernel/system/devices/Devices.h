/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/Func.h>
#include <libutils/Prelude.h>
#include "system/devices/DeviceAddress.h"
#include "system/devices/DeviceClass.h"
#include "system/devices/Driver.h"

void device_scan(IterFunc<DeviceAddress> address);
void device_iterate(IterFunc<RefPtr<Device>> callback);
void device_initialize();
void devices_acknowledge_interrupt(int interrupt);
void devices_handle_interrupt(int interrupt);
void device_mount(RefPtr<Device> device);

String device_claim_name(DeviceClass klass);