/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "buffer.h"
#include <devices/ata.h>
#include <memory/vmm.h>
#include <libkernel/math.h>

/**
 * @brief breed
 * 
 * @param devname 
 * @param sector 
 * @param size 
 * @return char* 
 */
char *bread(char *devname, sector_t sector, uint32_t size) {
	struct ata_device *device = get_ata_device(devname);
	char *buf = kcalloc(div_ceil(size, BYTES_PER_SECTOR) * BYTES_PER_SECTOR, sizeof(char));
	ata_read(device, sector, div_ceil(size, BYTES_PER_SECTOR), (uint16_t *)buf);
	return buf;
}

/**
 * @brief bwrite
 * 
 * @param devname 
 * @param sector 
 * @param buf 
 * @param size 
 */
void bwrite(char *devname, sector_t sector, char *buf, uint32_t size) {
	struct ata_device *device = get_ata_device(devname);
	ata_write(device, sector, div_ceil(size, BYTES_PER_SECTOR), (uint16_t *)buf);
}