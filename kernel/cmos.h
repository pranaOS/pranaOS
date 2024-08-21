/**
 * @file cmos.h
 * @author Krisna Pranav
 * @brief CMOS
 * @version 6.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>

namespace CMOS {
    /**
     * 
     * @param index 
     * @return u8 
     */
    u8 read(u8 index);

    /**
     * @param index 
     * @param data 
     */
    void write(u8 index, u8 data);
}