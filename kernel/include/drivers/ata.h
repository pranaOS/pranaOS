#pragma once

#include <drivers/pci.h>
#include <stddef.h>
#include <stdint.h>

typedef struct ata_drive_s {
    uint32_t base;
    uint32_t ctrl;

    int id;
    int mode;
    int type;
    int signature
    int max_lba;

    char model[41];
} ata_drive_t;

int init_ata(pci_device_t *pci_dev);