/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "system/Streams.h"
#include "ata/LegacyATA.h"
#include "system/scheduling/Scheduler.h"
#include "system/tasking/Task.h"

// bus
#define ATA_PRIMARY 0x00
#define ATA_SECONDARY 0x01

// drive
#define ATA_MASTER 0x00
#define ATA_SLAVE 0x01

// status flags
#define ATA_SR_BSY 0x80
#define ATA_SR_DRDY 0x40
#define ATA_SR_DF 0x20
#define ATA_SR_DSC 0x10
#define ATA_SR_DRQ 0x08
#define ATA_SR_CORR 0x04
#define ATA_SR_IDX 0x02
#define ATA_SR_ERR 0x01

// identify
#define ATA_IDENT_DEVICETYPE 0
#define ATA_IDENT_CYLINDERS 2
#define ATA_IDENT_HEADS 6
#define ATA_IDENT_SECTORS 12
#define ATA_IDENT_SERIAL 20
#define ATA_IDENT_MODEL 54
#define ATA_IDENT_NUM_BLOCKS0 60
#define ATA_IDENT_NUM_BLOCKS1 61
#define ATA_IDENT_LBA 83
#define ATA_IDENT_CAPABILITIES 98
#define ATA_IDENT_FIELDVALID 106
#define ATA_IDENT_MAX_LBA 120
#define ATA_IDENT_COMMANDSETS 164
#define ATA_IDENT_MAX_LBA_EXT 200

// commands
#define ATA_CMD_READ_PIO 0x20
#define ATA_CMD_READ_PIO_EXT 0x24
#define ATA_CMD_READ_DMA 0xC8
#define ATA_CMD_READ_DMA_EXT 0x25
#define ATA_CMD_WRITE_PIO 0x30
#define ATA_CMD_WRITE_PIO_EXT 0x34
#define ATA_CMD_WRITE_DMA 0xCA
#define ATA_CMD_WRITE_DMA_EXT 0x35
#define ATA_CMD_CACHE_FLUSH 0xE7
#define ATA_CMD_CACHE_FLUSH_EXT 0xEA
#define ATA_CMD_PACKET 0xA0
#define ATA_CMD_IDENTIFY_PACKET 0xA1
#define ATA_CMD_IDENTIFY 0xEC


// registers
#define ATA_REG_DATA 0x00
#define ATA_REG_ERROR 0x01
#define ATA_REG_FEATURES 0x01
#define ATA_REG_SECCOUNT0 0x02
#define ATA_REG_LBA0 0x03
#define ATA_REG_LBA1 0x04
#define ATA_REG_LBA2 0x05
#define ATA_REG_HDDEVSEL 0x06
#define ATA_REG_COMMAND 0x07
#define ATA_REG_STATUS 0x07
#define ATA_REG_SECCOUNT1 0x08
#define ATA_REG_LBA3 0x09
#define ATA_REG_LBA4 0x0A
#define ATA_REG_LBA5 0x0B
#define ATA_REG_CONTROL 0x0C
#define ATA_REG_ALTSTATUS 0x0C
#define ATA_REG_DEVADDRESS 0x0D

// io ports
#define ATA_PRIMARY_IO 0x1F0
#define ATA_SECONDARY_IO 0x170

// lba modes
#define ATA_28LBA_MAX 0x0FFFFFFF
#define ATA_48LBA_MAX 0xFFFFFFFFFFFF
#define ATA_SECTOR_SIZE 512

LegacyATA::LegacyATA(DeviceAddress address) : LegacyDevice(address, DeviceClass::DISK)
{
    switch (address.legacy())
    {
    case LEGACY_ATA0:
        _bus = ATA_PRIMARY;
        _drive = ATA_MASTER;
        break;
    case LEGACY_ATA1:
        _bus = ATA_PRIMARY;
        _drive = ATA_SLAVE;
        break;
    case LEGACY_ATA2:
        _bus = ATA_SECONDARY;
        _drive = ATA_MASTER;
        break;
    case LEGACY_ATA3:
        _bus = ATA_SECONDARY;
        _drive = ATA_SLAVE;
        break;

    default:
        break;
    }

    identify();
}
