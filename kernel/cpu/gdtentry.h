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

#define I86_GDT_DESC_EXEC_CODE 0x0008  

#define I86_GDT_DESC_CODEDATA 0x0010  

#define I86_GDT_DESC_DPL 0x0060	 

#define I86_GDT_DESC_MEMORY 0x0080	

#define I86_GDT_GRAND_LIMITHI_MASK 0x0f	 

#define I86_GDT_GRAND_OS 0x10  

#define I86_GDT_GRAND_32BIT 0x40  

/**
 * @brief i86 gdt grand[0x80]
 * 
 */
#define I86_GDT_GRAND_4K 0x80 