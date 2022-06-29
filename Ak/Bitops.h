#pragma once

#define ADDR (*(volatile int*)addr)

/**
 * @brief fzz
 * 
 * @param word 
 * @return unsigned int 
 */

static inline unsigned int ffz(unsigned int word) {
	__asm__("bsfl %1,%0"
			: "=r"(word)
			: "r"(~word));
	return word;
}

/**
 * @brief Set the bit object
 * 
 * @param nr 
 * @param addr 
 */

static inline void set_bit(int nr, volatile unsigned int* addr) {
	__asm__(
		"btsl %1,%0"
		: "=m"(ADDR)
		: "Ir"(nr));
}

/**
 * @brief clear bit function 
 * 
 * @param nr 
 * @param addr 
 */
static inline void clear_bit(int nr, volatile unsigned int* addr) {
	__asm__ __volatile__(
		"btrl %1,%0"
		: "=m"(ADDR)
		: "Ir"(nr));
}

/**
 * @brief mask addr test bit function 
 * 
 * @param nr 
 * @param addr 
 * @return int 
 */
static inline int test_bit(int nr, const unsigned int* addr) {
	int mask;

	addr += nr >> 5;
	mask = 1 << (nr & 0x1f);
	return ((mask & *addr) != 0);
}