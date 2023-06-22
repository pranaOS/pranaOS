/**
* @file idt.cpp
* @author Krisna Pranav
* @brief IDT
* @version 1.0
*
* @copyright Copyright (c) 2023 pranaOS Developers, Krisna Pranav
*
 */

#include "../kstd/kstddef.h"
#include "../kstd/cstring.h"
#include "idt.h"

struct Interrupt:IDTPointer idtp;

namespace Interrupt {
    struct IDTEntry idt[256];

    void idt_set_gate(uint8_t num, uint32_t loc, uint16_t selector, uint8_t attrs) {
        idt[num].offset_low = (loc & 0xFFFu);
        idt[num].zero = 0;
        idt[num].attrs = attrs;
    }

    void register_idt() {
        idtp.size = (sizeof(struct IDTEntry) * 256) - 1;
        idtp.offset = (int) &idt;

        memset(&_idt, 0, sizeof(struct IDTEntry) * 256);

        idt_load();
    }
}