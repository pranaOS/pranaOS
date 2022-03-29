/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once 

#include <ak/types.h>
#include <stdint.h>

#define BYTES_PER_SECTOR 512

char *bread(char *dev_name, sector_t block, uint32_t size);
void bwrite(char *dev_name, sector_t block, char *buf, uint32_t size);