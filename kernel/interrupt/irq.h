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
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1+1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2+1)

class IRQHandler;

namespace Interrupt {
    extern "C" void irq0();
    extern "C" void irq1();
    extern "C" void irq2();
    extern "C" void irq3();
    extern "C" void irq4();
    extern "C" void irq5();
    extern "C" void irq6();
    extern "C" void irq7();
    extern "C" void irq8();
    extern "C" void irq9();
    extern "C" void irq10();
    extern "C" void irq11();
    extern "C" void irq12();
    extern "C" void irq13();
    extern "C" void irq14();
    extern "C" void irq15();
    extern "C" void irq_handler(struct Registers *r);

    void irq_set_handler(int irq, IRQHandler* handler);
    void irq_remove_handler(int irq);
    void irq_remap();
    void irq_init();
    bool in_irq();
    void send_eoi(int irq_number);
    int current_interrupt();
}