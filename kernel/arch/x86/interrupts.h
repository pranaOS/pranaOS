/**
 * @file interrupts.h
 * @author Krisna Pranav
 * @brief Interrupts
 * @version 6.0
 * @date 2024-09-14
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>
#include <kernel/interrupt/genericinterrupthandler.h>
#include <mods/platform.h>

VALIDATE_IS_X86()

namespace Kernel 
{

    class GenericInterruptHandler;

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

    /**
     * @param number 
     * @param handler 
     */
    void register_interrupt_handler(u8 number, void (*handler)());

    /**
     * @param number 
     * @param handler 
     */
    void register_user_callable_interrupt_handler(u8 number, void (*handler)());

    /**
     * @brief Get the interrupt handler object
     * 
     * @param interrupt_number 
     * @return GenericInterruptHandler& 
     */
    GenericInterruptHandler& get_interrupt_handler(u8 interrupt_number);

    /**
     * @param number 
     */
    void register_generic_interrupt_handler(u8 number, GenericInterruptHandler&);

    /**
     * @param number 
     * @param handler 
     */
    void register_disabled_interrupt_handler(u8 number, GenericInterruptHandler& handler);

    /**
     * @param number 
     */
    void unregister_generic_interrupt_handler(u8 number, GenericInterruptHandler&);

    void idt_init();
} // namespace Kernel