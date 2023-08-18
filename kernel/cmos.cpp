/**
 * @file cmos.cpp
 * @author Krisna Pranav
 * @brief cmos
 * @version 6.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/cmos.h>
#include <kernel/io.h>

namespace CMOS {

    /**
     * @param index 
     * @return u8 
     */
    u8 read(u8 index) {
        IO::out8(0x70, index);
        return IO::in8(0x71);
    }
    
    /**
     * @param index 
     * @param data 
     */
    void write(u8 index, u8 data) {
        IO::out8(0x70, index);
        IO::out8(0x71, data);
    }

}