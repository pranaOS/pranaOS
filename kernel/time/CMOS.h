/**
* @file CMOS.h
* @author Krisna Pranav
* @brief CMOS
* @version 1.0
*
* @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
*
 */


#pragma once

#include "../kstd/kstddef.h"

#define CMOS_ADDRESS 0x70
#define CMOS_DATA 0x71

class CMOS {
public:
    /**
     * @brief write
     * 
     * @param reg 
     * @param val 
     */
    static void write(uint8_t reg, uint8_t val);

    /**
     * @brief read
     * 
     * @param reg 
     * @return uint8_t 
     */
    static uint8_t read(uint8_t reg);
};