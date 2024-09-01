/**
 * @file ksyms.h
 * @author Krisna Pranav
 * @brief KSYMS
 * @version 6.0
 * @date 2023-07-01
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/forward.h>

namespace Kernel
{
    struct KernelSymbol
    {
        FlatPtr address;
        const char* name;
    };

    enum class PrintToScreen
    {
        No,
        Yes,
    };

    /**
     * @brief address_for_kernel_symbol
     * @param name
     */
    FlatPtr address_for_kernel_symbol(StringView name);

    const KernelSymbol* symbolicate_kernel_address(FlatPtr);

    void load_kernel_symbol_table();

    extern bool g_kernel_symbols_available;

    extern bool g_kernel_symbols_available;

    extern FlatPtr g_lowest_kernel_symbol_address;

    extern FlatPtr g_highest_kernel_symbol_address;

    /**
     * @brief dump_backtrace
     * @param print_to_screen
     */
    void dump_backtrace(PrintToScreen print_to_screen = PrintToScreen::No);
} // namespace Kernel
