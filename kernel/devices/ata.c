/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <cpu/halt.h>
#include <cpu/idt.h>
#include <errno/error.h>
#include <memory/vmm.h>
#include <debug/debug.h>
#include <include/string.h>
#include "ata.h"

#define MAX_ATA_DEVICE 4

/**
 * @brief ata devices
 * 
 */
static struct ata_device devices[MAX_ATA_DEVICE];

/**
 * @brief number of actived devices
 * 
 */
static uint8_t number_of_actived_devices = 0;

/**
 * @brief ata irq called
 * 
 */
static volatile bool ata_irq_called;
