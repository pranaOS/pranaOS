/**
 * @file sections.h
 * @author Krisna Pranav
 * @brief sections
 * @version 6.0
 * @date 2024-09-11
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/platform.h>
#ifdef __cplusplus
#    include <mods/types.h>
#    include <kernel/bootinfo.h>
#endif

#define READONLY_AFTER_INIT __attribute__((section(".ro_after_init")))
#define UNMAP_AFTER_INIT NEVER_INLINE __attribute__((section(".unmap_after_init")))

#define KERNEL_PD_END (kernel_mapping_base + 0x31000000)
#define KERNEL_PT1024_BASE (kernel_mapping_base + 0x3FE00000)
#define KERNEL_QUICKMAP_PT (KERNEL_PT1024_BASE + 0x6000)
#define KERNEL_QUICKMAP_PD (KERNEL_PT1024_BASE + 0x7000)
#define KERNEL_QUICKMAP_PER_CPU_BASE (KERNEL_PT1024_BASE + 0x8000)

#define USER_RANGE_BASE 0x10000
#define USER_RANGE_CEILING (kernel_mapping_base - 0x2000000)
