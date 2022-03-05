/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <stdint.h>
#include "halt.h"

/**
 * @brief i86 max interrupts
 * 
 */
#define I86_MAX_INTERRUPTS 256

/**
 * @brief i86 idt
 * 
 */
#define I86_IDT_DESC_BIT16 0x06	   
#define I86_IDT_DESC_BIT32 0x0E	   
#define I86_IDT_DESC_RING1 0x40	   
#define I86_IDT_DESC_RING2 0x20	   
#define I86_IDT_DESC_RING3 0x60	  
#define I86_IDT_DESC_PRESENT 0x80 

#define DISPATCHER_ISR 0x7F

/**
 * @brief idt descriptor struct
 * 
 */
struct __attribute__((packed)) idt_descriptor {
	uint16_t baseLo;
	uint16_t sel;
	uint8_t reserved;
	uint8_t flags;
	uint16_t baseHi;
};

/**
 * @brief idtr
 * 
 */
struct __attribute__((packed)) idtr {
    uint16_t limit;
    uint32_t base;  
};