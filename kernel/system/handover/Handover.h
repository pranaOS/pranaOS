/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// inlcudes
#include "system/memory/MemoryRange.h"

#define HANDOVER_BOOTLOADER_NAME_SIZE 64
#define HANDOVER_COMMAND_LINE_SIZE 256
#define HANDOVER_MEMORY_MAP_SIZE 64
#define HANDOVER_MODULES_SIZE 16

enum MemoryMapEntryType
{
    MEMORY_MAP_ENTRY_AVAILABLE,
    MEMORY_MAP_ENTRY_RESERVED,
    MEMORY_MAP_ENTRY_ACPI_RECLAIMABLE,
    MEMORY_MAP_ENTRY_NVS,
    MEMORY_MAP_ENTRY_BADRAM,
    MEMORY_MAP_ENTRY_KERNEL
};