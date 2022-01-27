/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: GPL-3.0 License
 */

#include "print.h"

using namespace ak;
using namespace Kernel;

void Print::printfHex(uint8_t key) {
    char *foo = "00";
    char *hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    system::BootConsole::write(foo);
}

void Print::printfHex16(uint16_t key) {
    printfHex((key >> 8) & 0xFF);
    printfHex(key & 0xFF);
}

void Print::printfHex32(uint32_t key) {
    printfHex((key >> 24) & 0xFF);
    printfHex((key >> 16) & 0xFF);
    printfHex((key >> 8) & 0xFF);
    printfHex(key & 0xFF);
}

void Print::printbits(uint8_t key) {
    for (unsigned int bit = 0; bit < (sizeof(key)*8); bit++) {
        bootConsole::write(Convert::intToString(key & 0x01));
        key = key >> 1;
    }
}

void Print::printbits(uint16_t key) {
    for (unsigned int bit = 0; bit < (sizeof(key)*8); bit++) {
        bootConsole::write(Convert::intToString(key & 0x01));
        key = key >> 1;
    }
}

void Print::printbits(uint32_t key) {
    for (unsigned int bit = 0; bit < (sizeof(key)*8); bit++) {
        bootConsole::write(convert::intToString(key & 0x01));
        key = key >> 1;
    }
}

void Print::printbits(uint64_t key) {
    for (unsigned int bit = 0; bit < (sizeof(key)*8); bit++) {
        bootConsole::write(Convert::intToString(key & 0x01));
        key = key >> 1;
    }
}