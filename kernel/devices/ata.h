/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

#define ATA0_IO_ADDR1 0x1F0
#define ATA0_IO_ADDR2 0x3F0
#define ATA0_IRQ 14
#define ATA1_IO_ADDR1 0x170
#define ATA1_IO_ADDR2 0x370
#define ATA1_IRQ 15

#define ATA_SREG_ERR 0x01
#define ATA_SREG_DF 0x20
#define ATA_SREG_DRQ 0x08
#define ATA_SREG_BSY 0x80

#define ATA_POLLING_ERR 0
#define ATA_POLLING_SUCCESS 1

#define ATA_IDENTIFY_ERR 0
#define ATA_IDENTIFY_SUCCESS 1
#define ATA_IDENTIFY_NOT_FOUND 2