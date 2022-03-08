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

/**
 * @brief ata 400 delays
 * 
 * @param device 
 */
static void ata_400_delays(struct ata_device *device) {
	inportb(device->io_base + 7);
	inportb(device->io_base + 7);
	inportb(device->io_base + 7);
	inportb(device->io_base + 7);
}

/**
 * @brief ata polling
 * 
 * @param device 
 * @return uint8_t 
 */
static uint8_t ata_polling(struct ata_device *device) {
	uint8_t status;

	while (true) {
		status = inportb(device->io_base + 7);
		if (!(status & ATA_SREG_BSY) || (status & ATA_SREG_DRQ))
			return ATA_POLLING_SUCCESS;
		if ((status & ATA_SREG_ERR) || (status & ATA_SREG_DF))
			return ATA_POLLING_ERR;
	}
}