/**
 * @file utils.cpp
 * @author Krisna Pranav
 * @brief Utils
 * @version 6.0
 * @date 2024-10-20
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/prekernel/arch/aarch64/uart.h>
#include <kernel/prekernel/arch/aarch64/utils.h>

/**
 * @param text 
 */
void Prekernel::dbgln(const char* text)
{
    auto& uart = Prekernel::UART::the();
    
    uart.print_str(text);
    uart.print_str("\r\n");
}

/**
 * @param text 
 */
void Prekernel::warnln(const char* text)
{
    dbgln(text);
}
