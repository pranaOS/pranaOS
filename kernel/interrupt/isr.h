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
    void isr_init();

    extern "C" void fault_handler(struct Registers* r);
}

void print_regs(struct Registers *r);