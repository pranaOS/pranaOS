/**
* @file CMOS.cpp
* @author Krisna Pranav
* @brief CMOS
* @version 1.0
*
* @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
*
 */

#include "CMOS.h"
#include "../io.h"

void CMOS::write(uint8_t reg, uint8_t val) {
    IO::outb(CMOS_ADDRESS, reg);
    IO::outb(CMOS_DATA, val);
}

uint8_t CMOS::read(uint8_t reg) {
    IO::outb(CMOS_ADDRESS, reg);
    return IO::inb(CMOS_DATA);
}