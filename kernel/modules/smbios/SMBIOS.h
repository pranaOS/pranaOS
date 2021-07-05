/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <string.h>
#include "system/memory/MemoryRange.h"

namespace Smbios
{

#define SMBIOS_HEADER_TYPE_LIST(__ENTRY)                 \
    __ENTRY(BIOS_INFORMATION, 0)                         \
    __ENTRY(SYSTEM_INFORMATION, 1)                       \
    __ENTRY(BASEBOARD_OR_MODULE_INFORMATION, 2)          \
    __ENTRY(SYSTEM_ENCLOSURE_OR_CHASSIS, 3)              \
    __ENTRY(PROCESSOR_INFORMATION, 4)                    \
    __ENTRY(MEMORY_CONTROLLER_INFORMATION, 5)            \
    __ENTRY(MEMORY_MODULE_INFORMATION, 6)                \
    __ENTRY(CACHE_INFORMATION, 7)                        \
    __ENTRY(PORT_CONNECTOR_INFORMATION, 8)               \
    __ENTRY(SYSTEM_SLOTS, 9)                             \
    __ENTRY(ON_BOARD_DEVICES_INFORMATION, 10)            \
    __ENTRY(OEM_STRINGS, 11)                             \
    __ENTRY(SYSTEM_CONFIGURATION_OPTIONS, 12)            \
    __ENTRY(BIOS_LANGUAGE_INFORMATION, 13)               \
    __ENTRY(GROUP_ASSOCIATIONS, 14)                      \
    __ENTRY(SYSTEM_EVENT_LOG, 15)                        \
    __ENTRY(PHYSICAL_MEMORY_ARRAY, 16)                   \
    __ENTRY(MEMORY_DEVICE, 17)                           \
    __ENTRY(MEMORY_ERROR_INFORMATION_32_BIT, 18)         \
    __ENTRY(MEMORY_ARRAY_MAPPED_ADDRESS, 19)             \
    __ENTRY(MEMORY_DEVICE_MAPPED_ADDRESS, 20)            \
    __ENTRY(BUILT_IN_POINTING_DEVICE, 21)                \
    __ENTRY(PORTABLE_BATTERY, 22)                        \
    __ENTRY(SYSTEM_RESET, 23)                            \
    __ENTRY(HARDWARE_SECURITY, 24)                       \
    __ENTRY(SYSTEM_POWER_CONTROLS, 25)                   \
    __ENTRY(VOLTAGE_PROBE, 26)                           \
    __ENTRY(COOLING_DEVICE, 27)                          \
    __ENTRY(TEMPERATURE_PROBE, 28)                       \
    __ENTRY(ELECTRICAL_CURRENT_PROBE, 29)                \
    __ENTRY(OUT_OF_BAND_REMOTE_ACCESS, 30)               \
    __ENTRY(BOOT_INTEGRITY_SERVICES_BIS_ENTRY_POINT, 31) \
    __ENTRY(SYSTEM_BOOT_INFORMATION, 32)                 \
    __ENTRY(MEMORY_ERROR_INFORMATION_64_BIT, 33)         \
    __ENTRY(MANAGEMENT_DEVICE, 34)                       \
    __ENTRY(MANAGEMENT_DEVICE_COMPONENT, 35)             \
    __ENTRY(MANAGEMENT_DEVICE_THRESHOLD_DATA, 36)        \
    __ENTRY(MEMORY_CHANNEL, 37)                          \
    __ENTRY(IPMI_DEVICE_INFORMATION, 38)                 \
    __ENTRY(SYSTEM_POWER_SUPPLY, 39)                     \
    __ENTRY(ADDITIONAL_INFORMATION, 40)                  \
    __ENTRY(ONBOARD_DEVICES_EXTENDED_INFORMATION, 41)    \
    __ENTRY(MANAGEMENT_CONTROLLER_HOST_INTERFACE, 42)    \
    __ENTRY(TPM_DEVICE, 43)                              \
    __ENTRY(PROCESSOR_ADDITIONAL_INFORMATION, 44)        \
    __ENTRY(INACTIVE, 126)                               \
    __ENTRY(END_OF_TABLE, 127)


enum struct HearderType : uint8_t
{

#define __ENTRY(__name, __id) \
    __name = __id,

    SMBIOS_HEADER_TYPE_LIST(__ENTRY)
#undef __ENTRY

};

}