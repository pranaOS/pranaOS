#pragma once

#include <drivers/pci.h>

typedef struct {
	pci_device_t *pci_dev;
	int           bar_type;
	uint32_t      io_base;
	uint32_t      mem_base;
	void *        rx_buffer;
	int           tx_current;
	int           current_packet_offset;
} rtl8139_dev_t;

int init_rtl8139(pci_device_t *pcidev);