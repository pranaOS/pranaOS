/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Types.h>
#include <kernel/interrupts/GenericInterruptHandler.h>

namespace Kernel {

class GenericInterruptHandeler;

extern "C" void interrupt_common_asm_entry();

#if ARCH(I386)
#    define INTERRUPT_HANDLER_PUSH_PADDING
#else
#    define INTERRUPT_HANDLER_PUSH_PADDING "pushw $0\npushw $0\n"
#endif

#define GENERATE_GENERIC_INTERRUPT_HANDLER_ASM_ENTRY(isr_number)                  \
    extern "C" void interrupt_##isr_number##_asm_entry();                         \
    static void interrupt_##isr_number##_asm_entry_dummy() __attribute__((used)); \
    NEVER_INLINE void interrupt_##isr_number##_asm_entry_dummy()                  \
    {                                                                             \
        asm(".globl interrupt_" #isr_number "_asm_entry\n"                        \
            "interrupt_" #isr_number "_asm_entry:\n"                              \
            INTERRUPT_HANDLER_PUSH_PADDING                                        \
            "    pushw $" #isr_number "\n"                                        \
            "    pushw $0\n"                                                      \
            "    jmp interrupt_common_asm_entry\n");                              \
    }

void register_interrupt_handler(u8 number, void (*handler)());
void register_user_callable_interrupt_handler(u8 number, void (*handler)());
GenericInterruptHandler& get_interrupt_handler(u8 interrupt_number);
void register_generic_interrupt_handler(u8 number, GenericInterruptHandler&);
void unregister_generic_interrupt_handler(u8 number, GenericInterruptHandler&);

void idt_init();

}