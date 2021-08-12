/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Forward.h>

namespace Kernel {

struct KernelSymbol {
    FlatPtr address;
    const char* name;
};

enum class PrintToScreen {
    No,
    Yes,
};

FlatPtr address_for_kernel_symbol(const StringView& name);
const KernelSymbol* symbolicate_kernel_address(FlatPtr);
void load_kernel_symbol_table();

extern bool g_kernel_symbols_available;
extern FlatPtr g_lowest_kernel_symbol_address;
extern FlatPtr g_highest_kernel_symbol_address;

void dump_backtrace(PrintToScreen print_to_screen = PrintToScreen::No);

}
