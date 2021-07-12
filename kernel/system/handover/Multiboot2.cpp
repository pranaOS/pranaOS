/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include "system/Streams.h"
#include <assert.h>
#include <string.h>

#include <extramodules/multiboot/Multiboot2.h>
#include "system/handover/Handover.h"

bool is_multiboot2(uint32_t magic)
{
    return magic == MULTIBOOT2_BOOTLOADER_MAGIC;
}

const char *_multiboot2_tag_name[] = {
    "END",
    "CMDLINE",
    "BOOT_LOADER_NAME",
    "MODULE",
    "BASIC_MEMINFO",
    "BOOTDEV",
    "MMAP",
    "VBE",
    "FRAMEBUFFER",
    "ELF_SECTIONS",
    "APM",
    "EFI32",
    "EFI64",
    "SMBIOS",
    "ACPI_OLD",
    "ACPI_NEW",
    "NETWORK",
    "EFI_MMAP",
    "EFI_BS",
    "EFI32_IH",
    "EFI64_IH",
    "LOAD_BASE_ADDR",
};