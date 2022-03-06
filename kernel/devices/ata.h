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

/**
 * @brief ata device
 * 
 */
struct ata_device {

    /**
     * @brief io base + associated io base
     * 
     */
    uint16_t io_base;
    uint16_t associated_io_base;

    /**
     * @brief irq 
     * 
     */
    uint8_t irq;

    /**
     * @brief dev name
     * 
     */
    char *dev_name;

    /**
     * @brief is master, harddisk
     * 
     */
    bool is_master;
    bool is_harddisk;
};

/**
 * @brief ata read
 * 
 * @param device 
 * @param lba 
 * @param n_sectors 
 * @param buffer 
 * @return int8_t 
 */
int8_t ata_read(struct ata_device *device, uint32_t lba, uint32_t n_sectors, uint16_t *buffer);

/**
 * @brief ata write
 * 
 * @param device 
 * @param lba 
 * @param n_sectors 
 * @param buffer 
 * @return int8_t 
 */
int8_t ata_write(struct ata_device *device, uint32_t lba, uint32_t n_sectors, uint16_t *buffer);

/**
 * @brief atapi read 
 * 
 * @param device 
 * @param lba 
 * @param n_sectors 
 * @param buffer 
 * @return int8_t 
 */
int8_t atapi_read(struct ata_device *device, uint32_t lba, uint8_t n_sectors, uint16_t *buffer);

struct ata_device *get_ata_device(char *dev_name);

/**
 * @brief initialize function
 * 
 * @return uint8_t 
 */
uint8_t ata_init();