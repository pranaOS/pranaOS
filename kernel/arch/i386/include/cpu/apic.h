#pragma once

#include <stdint.h>

struct madt {
	uint32_t signature;
	uint32_t length;
	uint8_t  revistion;
	uint8_t  checksum;
	uint8_t  OEMID[6];
	uint8_t  OEM_table_id[8];
	uint32_t OEM_rev;
	uint32_t creator_id;
	uint32_t creator_rev;

	uint32_t local_apic;
	uint32_t flags;

} __attribute__((packed));

int  init_apic();
void apic_send_eio(int inter);