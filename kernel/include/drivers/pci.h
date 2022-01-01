#pragma once

#include <stdint.h>

#if 0 

	#define PCI_UNCLASSIFIED                       0x00
	#define PCI_MASS_STORAGE_CONTROLLER            0x01
	#define PCI_NETWORK_CONTROLLER                 0x02
	#define PCI_DISPLAY_CONTROLLER                 0x03
	#define PCI_MULTIMEDIA_CONTROLLER              0x04
	#define PCI_MEMORY_CONTROLLER                  0x05
	#define PCI_BRIDGE_DEVICE                      0x06
	#define PCI_SIMPLE_COMMUNICATION_CONTROLLER    0x07
	#define PCI_BASE_SYSTEM_PERIPHERAL             0x08
	#define PCI_INPUT_DEVICE_CONTROLLER            0x09
	#define PCI_DOCKING_STATION                    0x0A
	#define PCI_PROCESSOR                          0x0B
	#define PCI_SERIAL_BUS_CONTROLLER              0x0C
	#define PCI_WIRELESS_CONTROLLER                0x0D
	#define PCI_INTELLIGENT_CONTROLLER             0x0E
	#define PCI_SATELLITE_COMMUNICATION_CONTROLLER 0x0F
	#define PCI_ENCRYPTION_CONTROLLER              0x10
	#define PCI_SIGNAL_PROCESSING_CONTROLLER       0x11
	#define PCI_PROCESSING_ACCELERATOR             0x12
	#define PCI_NON_ESSENTIAL_INSTRUMENTATION      0x13
	#define PCI_CO_PROCESSOR                       0x40
	#define PCI_UNASSIGNED_CLASS                   0xFF

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