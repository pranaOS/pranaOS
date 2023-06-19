/**
 * @file io.cpp
 * @author Krisna Pranav
 * @brief IO
 * @version 1.0
 * @date 2023-06-19
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include "io.h"

/**
 * @brief outb
 * 
 * @param port 
 * @param value 
 */
void IO::outb(uint16_t port, uint8_t value) {
    asm volatile("outb %1, %0" : : "d"(port), "a" (value));
}

/**
 * @brief inb
 * 
 * @param port 
 * @return uint8_t 
 */
uint8_t IO::inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}