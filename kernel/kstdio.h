/**
 * @file kstdio.h
 * @author Krisna Pranav 
 * @brief KSTDIO
 * @version 6.0
 * @date 2023-07-01
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>

extern "C" {

    /**
     * @param fmt 
     * @param ... 
     * @return int 
     */
    int dbgprintf(const char* fmt, ...);

    /**
     * @return int 
     */
    int dbgputstr(const char*, int);

    /**
     * @return int 
     */
    int kernelputstr(const char*, int);

    /**
     * @param fmt 
     * @param ... 
     * @return int 
     */
    int kprintf(const char* fmt, ...);

    /**
     * @param buf 
     * @param fmt 
     * @param ... 
     * @return int 
     */
    int snprintf(char* buf, size_t, const char* fmt, ...);

    /**
     * @param on_or_off 
     */
    void set_serial_debug(bool on_or_off);

    /**
     * @return int 
     */
    int get_serial_debug();
}