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

#define PCI_VENDOR_OFFSET         0x0
#define PCI_DEVICE_ID_OFFSET      0x2
#define PCI_COMMAND_OFFSET        0x4
#define PCI_INTERRUPT_LINE_OFFSET 0x3C
#define PCI_INTERRUPT_PIN_OFFSET  0x3E

#define PCI_BAR_MEM  0x0
#define PCI_BAR_IO   0x1
#define PCI_BAR_NONE 0x3

struct pci_device_s;
typedef struct pci_device_s pci_device_t;

typedef int (*pci_device_initialiser_t)(pci_device_t *pci_dev);

struct pci_device_s
{
	uint16_t bus;
	uint16_t device;
	uint16_t function;

	uint16_t                 vendor_id;
	uint16_t                 device_id;
	uint16_t                 command;
	uint8_t                  class_code;
	uint8_t                  subclass;
	uint8_t                  prog_if;
	uint8_t                  rev_id;
	uint8_t                  header_type;
	void *                   header;
	pci_device_initialiser_t driver;
	const char *             name;
	struct pci_device_s *    next;
};

typedef struct
{
	uint32_t BAR[6];
	uint8_t  interrupt_line;
	uint8_t  interrupt_pin;
	uint8_t  min_grant;
	uint8_t  max_latency;
} pci_common_header_t;

void pci_write_dword(uint16_t bus, uint16_t slot, uint16_t function,
                     uint16_t offset, uint32_t value);

pci_device_t *pci_find_by_class(int classcode, int subclass, int prog_if);

pci_device_t *pci_find_by_vendor(int vendor, int device);

uint32_t pci_get_bar_by_index(pci_device_t *pcidev, uint32_t index);
uint32_t pci_get_bar_by_type(pci_device_t *pcidev, uint8_t type);
int init_pci();
int init_pci_devices();