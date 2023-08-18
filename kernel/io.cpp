/**
 * @file io.cpp
 * @author Krisna Pranav
 * @brief IO
 * @version 6.0
 * @date 2023-06-19
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include "io.h"

/**
 * @brief wait
 * 
 */
void IO::wait() {
	asm volatile ( "jmp 1f\n\t"
				   "1:jmp 2f\n\t"
				   "2:" );
}

/**
 * @brief outb
 * 
 * @param port 
 * @param value 
 */
void IO::outb(uint16_t port, uint8_t value){
	asm volatile ("outb %1, %0" : : "d" (port), "a" (value));
}

/**
 * @brief outw
 * 
 * @param port 
 * @param value 
 */
void IO::outw(uint16_t port, uint16_t value){
	asm volatile ("outw %1, %0" : : "dN" (port), "a" (value));
}

/**
 * @brief outl
 * 
 * @param port 
 * @param value 
 */
void IO::outl(uint16_t port, uint32_t value){
	asm volatile ("outl %1, %0" : : "dN" (port), "a" (value));
}

/**
 * @brief inb
 * 
 * @param port 
 * @return uint8_t 
 */
uint8_t IO::inb(uint16_t port){
	uint8_t ret;
	asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

/**
 * @brief inw
 * 
 * @param port 
 * @return uint16_t 
 */
uint16_t IO::inw(uint16_t port){
	uint16_t ret;
	asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}


/**
 * @brief inl
 * 
 * @param port 
 * @return uint32_t 
 */
uint32_t IO::inl(uint16_t port){
	uint32_t ret;
	asm volatile ("inl %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}