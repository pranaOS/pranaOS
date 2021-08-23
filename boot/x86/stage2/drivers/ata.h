#ifndef _BOOT_X86_STAGE2_DRIVERS_ATA_H
#define _BOOT_X86_STAGE2_DRIVERS_ATA_H

#include "../types.h"
#include "drive_desc.h"
#include "port.h"

typedef struct { 
    uint32_t data_port;
    uint32_t error_port; 
    uint32_t sector_count_port;
    uint32_t lba_lo_port; 
    uint32_t lba_mid_port;
    uint32_t lba_hi_port; 
    uint32_t device_port; 
    uint32_t command_port; 
    uint32_t control_port;
    char is_master;
} ata_t;

void init_ata(uint32_t port, char is_master);
int indentify_ata_device(drive_desc_t* drive_desc);
int ata_read(uint32_t sector, uint8_t* read_to);

#endif