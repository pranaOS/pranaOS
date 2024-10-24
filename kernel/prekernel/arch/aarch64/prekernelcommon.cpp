/**
 * @file prekernelcommon.cpp
 * @author Krisna Pranav
 * @brief PreKernelCommon
 * @version 6.0
 * @date 2024-10-24
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/arch/aarch64/asm_wrapper.h>
#include <kernel/prekernel/arch/aarch64/prekernel.h>
#include <kernel/prekernel/arch/aarch64/uart.h>

namespace Prekernel 
{
    /**
     * @param msg 
     */
    [[noreturn]] void panic(const char* msg)
    {
        auto& uart = Prekernel::UART::the();

        if (msg) {
            uart.print_str(msg);
        }

        Prekernel::halt();
    }

    [[noreturn]] void halt()
    {
        for (;;) {
            asm volatile("wfi");
        }
    }
} // namespace Prekernel
