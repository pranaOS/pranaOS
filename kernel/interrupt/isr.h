/**
* @file isr.h
* @author Krisna Pranav
* @brief isr
* @version 1.0
*
* @copyright Copyright (c) 2023 pranaOS Developers, Krisna Pranav
*
 */

#pragma once

#include "../kstd/kstddef.h"

#define FAULT_KERNEL_READ 0
#define FAULT_KERNEL_READ_GPF 1
#define FAULT_KERNEL_WRITE 2
#define FAULT_KERNEL_WRITE_GPF 3
#define FAULT_USER_READ 4
#define FAULT_USER_READ_GPF 5
#define FAULT_USER_WRITE 6
#define FAULT_USER_WRITE_GPF 7

namespace Interrupt {
    extern "C" void isr0();
    extern "C" void isr1();
    extern "C" void isr2();
    extern "C" void isr3();
    extern "C" void isr4();
    extern "C" void isr5();
    extern "C" void isr6();
    extern "C" void isr7();
    extern "C" void isr8();
    extern "C" void isr9();
    extern "C" void isr10();
    extern "C" void isr11();
    extern "C" void isr12();
    extern "C" void isr13();
    extern "C" void isr14();
    extern "C" void isr15();
    extern "C" void isr16();
    extern "C" void isr17();
    extern "C" void isr18();
    extern "C" void isr19();
    extern "C" void isr20();
    extern "C" void isr21();
    extern "C" void isr22();
    extern "C" void isr23();
    extern "C" void isr24();
    extern "C" void isr25();
    extern "C" void isr26();
    extern "C" void isr27();
    extern "C" void isr28();
    extern "C" void isr29();
    extern "C" void isr30();
    extern "C" void isr31();

    void isr_init();

    extern "C" void fault_handler(struct Registers *r);
}

void print_regs(struct Registers *r);