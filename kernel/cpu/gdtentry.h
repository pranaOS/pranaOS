/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <stdint.h>

/**
 * @brief max descriptors [6]
 * 
 */
#define MAX_DESCRIPTORS 6

/**
 * @brief define i86 gdt description access[0001]
 * 
 */
#define I86_GDT_DESC_ACCESS 0x0001

/**
 * @brief i86 gdt description readwrite[0002]
 * 
 */
#define I86_GDT_DESC_READWRITE 0x0002  

/*
 * @brief i86 gdt description expanision[0004]
 * 
 */
#define I86_GDT_DESC_EXPANSION 0x0004  

/**
 * @brief i86 gdt description exec code[0008]
 * 
 */
#define I86_GDT_DESC_EXEC_CODE 0x0008  

/**
 * @brief i86 gdt description codedata[0010]
 * 
 */
#define I86_GDT_DESC_CODEDATA 0x0010  

/**
 * @brief i86 gdt description dpl[0060]
 * 
 */
#define I86_GDT_DESC_DPL 0x0060	 

/**
 * @brief i86 gdt desc memory[0080]
 * 
 */
#define I86_GDT_DESC_MEMORY 0x0080	

/**
 * @brief i86 gdt grand limit_mask[0x0f]
 * 
 */
#define I86_GDT_GRAND_LIMITHI_MASK 0x0f	 

/**
 * @brief i86 gdt grand os[0x10]
 * 
 */
#define I86_GDT_GRAND_OS 0x10  

/**
 * @brief i86 gdt grand 32bit[0x40]
 * 
 */
#define I86_GDT_GRAND_32BIT 0x40  

/**
 * @brief i86 gdt grand[0x80]
 * 
 */
#define I86_GDT_GRAND_4K 0x80 

/**
 * @brief gdtdescriptor[attribute packed class] limit, baseLo, baseMid, flags, grand, base
 * 
 */
struct __attribute__((packed)) gdt_descriptor {
	uint16_t limit;

	uint16_t baseLo;
	uint8_t baseMid;

	uint8_t flags;

	uint8_t grand;

	uint8_t base;
};

/**
 * @brief gdtr
 * 
 */
struct __attribute__((packed)) gdtr {
    uint16_t limit;
    uint32_t base;
};

/**
 * @brief gdt init class
 * 
 */
void gdt_init();

/**
 * @brief gdt set descriptor
 * 
 * @param i 
 * @param base 
 * @param limit 
 * @param access 
 * @param grand 
 */
void gdt_set_descriptor(uint32_t i, uint64_t base, uint64_t limit, uint8_t access, uint8_t grand);