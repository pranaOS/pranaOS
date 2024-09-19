/**
 * @file panic.h
 * @author Krisna Pranav
 * @brief panic
 * @version 6.0
 * @date 2024-09-19
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace Kernel 
{

    /**
     * @param file 
     * @param line 
     * @param function 
     */
    [[noreturn]] void __panic(const char* file, unsigned int line, const char* function);

    #define PANIC(...)                                        \
        do {                                                  \
            critical_dmesgln("KERNEL PANIC! :^(");            \
            critical_dmesgln(__VA_ARGS__);                    \
            __panic(__FILE__, __LINE__, __PRETTY_FUNCTION__); \
        } while (0)

} // namespace Kernel