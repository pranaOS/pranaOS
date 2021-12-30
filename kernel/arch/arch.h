/**
 * @file arch.h
 * @author Krisna Pranav
 * @brief 
 * @version 0.1
 * @date 2021-12-30
 * 
 * @copyright Copyright (c) 2021 GPL-3.0
 * 
 */

#pragma once

#include <cstdint>

namespace arch {

    void interrupts_enable();
    void interrupts_disable();
    
    void halt();

    void stacktrace_dump();

    void paging_init();
    void paging_map(uintptr_t virtual_addr, uintptr_t phys_addr, uint32_t flags);

    void early_print(const char* s);
    void early_print_char(char c);
    char early_getchar();
}