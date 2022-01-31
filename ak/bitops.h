#pragma once

#define ADDR (*(volatile int*)addr)

static inline unsigned int ffz(unsigned int word) {
    __asm__("bsfl %1,%0"
            : "=r"(word)
            : "r"(~word));

    return word
}

static inline test_bit(int nr, const unsigned int* addr) {
    int mask;
    addr += nr >> 5;
    mask = 1 << (nr & 0x1f);
    return ((mask & *addr) != 0);
}