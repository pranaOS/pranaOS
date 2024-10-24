/**
 * @file prekernel.h
 * @author Krisna Pranav
 * @brief PreKernel
 * @version 6.0
 * @date 2024-10-24
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace Prekernel 
{
    void drop_to_exception_level_1();
    void init_prekernel_page_tables();

    /**
     * @param msg 
     */
    [[noreturn]] void panic(const char* msg);

    [[noreturn]] void halt();
}
