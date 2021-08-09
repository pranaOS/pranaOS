/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// incldues
#include <platform/x86/port.h>

uint8_t port_byte_in(uint16_t port)
{
    uint8_t result_data;
    asm volatile("inb %%dx, %%al"
                 : "=a"(result_data)
                 : "d"(port));
    return result_data;
}

void port_byte_out(uint16_t port, uint8_t data)
{
    asm volatile("outb %%al, %%dx"
                 :
                 : "a"(data), "d"(port));
}

uint16_t port_word_in(uint16_t port)
{
    uint16_t result_data;
    asm volatile("inw %%dx, %%ax"
                 : "=a"(result_data)
                 : "d"(port));
    return result_data;
}

void port_word_out(uint16_t port, uint16_t data)
{
    asm volatile("outw %%ax, %%dx"
                 :
                 : "a"(data), "d"(port));
}