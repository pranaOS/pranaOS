/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <cpu/port.h>

/**
 * @brief static unsigend values
 * 
 */
static unsigned COMREG;
static unsigned int seconds;
static unsigned int ticks;

/**
 * @brief sleep 
 * 
 * @param secs 
 * @return unsigned int 
 */
unsigned int sleep(unsigned int secs) {
    int sleeptimer = seconds + secs;

    while (ticks != 0);
    while (seconds < sleeptimer);
    
    return 0;
}

/**
 * @brief initialize function
 * 
 */
void init() {
    asm("cli");
    Kernel::outportb(COMREG, 0x37);
    asm("sti");
}