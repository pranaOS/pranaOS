/**
* @file irq.h
* @author Krisna Pranav
* @brief irqhandler
* @version 1.0
*
* @copyright Copyright (c) 2023 pranaOS Developers, Krisna Pranav
*
 */

#pragma once

#define PIC1 0x20
#define PIC2 0xA0

class IRQHandler;

namespace Interrupt {
    extern "C" void irq_handler(struct Registers* reg);

    void irq_set_handler(int irq, IRQHandler* handler);
    void irq_remove_handler(int req);
    void irq_remap();
    void irq_init();
    bool in_irq();
    void send_eoi(int irq_number);
    int current_interrupt();
}