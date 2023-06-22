/**
* @file idt.h
* @author Krisna Pranav
* @brief IDT
* @version 0.1
* @date 2023-05-29
*
* @copyright Copyright (c) 2023 pranaOS Developers, Krisna Pranav
*
*/

#pragma once

namespace Interrupt {
    struct IDTEntry {
        unsigned short offset_low;
        unsigned short selector;
        unsigned char zero;
        unsigned char attrs;
        unsigned short offset_high;
    } __attribute__((packed));

    struct IDTPointer {
        unsigned short size;
    } __attribute__((packed));

    extern "C" void idt_load();

    void idt_set_gate(uint8_t num, uint32_t loc, uint16_t selector, uint8_t flags);

    void register_idt();
}