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

