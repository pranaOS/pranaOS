#pragma once

#include <stdint.h>

#if 0

#endif 

#define PCI_VENDOR_OFFSET 0x0
#define PCI_DEVICE_ID_OFFSET 0x2 
#define PCI_COMMAND_OFFSET 0x4
#define PCI_INTERRUPT_LINE_OFFSET 0x3C
#define PCI_INTERRUPT_PIC_OFFSET 0x3E

#define PCI_BAR_MEM 0x0
#define PCI_BAR_IO 0x1
#define PCI_BAR_NONE 0x3 

struct pci_device_s;
typedef struct pci_device_s pci_device_t;

typedef int (*pci_device_initializer_t)(pci_device_t *pci_dev);

struct pci_device_s
{
    uint16_t bus;
    uint16_t device;
    uint16_t function;

    const char* name;
};