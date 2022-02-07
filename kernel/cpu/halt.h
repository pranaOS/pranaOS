/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <ak/cdefs.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief io_wait
 * 
 * @return __inline 
 */
static __inline void io_wait() {
	__asm__ __volatile__("outb %%al, $0x80"
						 :
						 : "a"(0));
}

/**
 * @brief enable_interrupts
 * 
 * @return __inline 
 */
static __inline void enable_interrupts() {
	__asm__ __volatile__("sti");
}

/**
 * @brief disable_interrupts
 * 
 * @return __inline 
 */
static __inline void disable_interrupts() {
	__asm__ __volatile__("cli");
}

/**
 * @brief halt
 * 
 * @return __inline 
 */
static __inline void halt() {
	__asm__ __volatile__("hlt");
}

/**
 * @brief inportb
 * 
 * @param _port 
 * @return __inline 
 */
static __inline unsigned char inportb(unsigned short _port) {
	unsigned char rv;
	asm volatile("inb %1, %0"
				 : "=a"(rv)
				 : "dN"(_port));
	return rv;
}

/**
 * @brief outportb
 * 
 * @param _port 
 * @param _data 
 * @return __inline 
 */
static __inline void outportb(unsigned short _port, unsigned char _data) {
	asm volatile("outb %1, %0"
				 :
				 : "dN"(_port), "a"(_data));
}

/**
 * @brief inportw
 * 
 * @param _port 
 * @return __inline 
 */
static __inline unsigned short inportw(unsigned short _port) {
	unsigned short rv;
	asm volatile("inw %1, %0"
				 : "=a"(rv)
				 : "dN"(_port));
	return rv;
}

/**
 * @brief outportw
 * 
 * @param _port 
 * @param _data 
 * @return __inline 
 */
static __inline void outportw(unsigned short _port, unsigned short _data) {
	asm volatile("outw %1, %0"
				 :
				 : "dN"(_port), "a"(_data));
}

/**
 * @brief inportl
 * 
 * @param _port 
 * @return __inline 
 */
static __inline unsigned int inportl(unsigned short _port) {
	unsigned int rv;
	asm volatile("inl %%dx, %%eax"
				 : "=a"(rv)
				 : "dN"(_port));
	return rv;
}

/**
 * @brief outportl
 * 
 * @param _port 
 * @param _data 
 * @return __inline 
 */
static __inline void outportl(unsigned short _port, unsigned int _data) {
	asm volatile("outl %%eax, %%dx"
				 :
				 : "dN"(_port), "a"(_data));
}

/**
 * @brief inports
 * 
 * @param _port 
 * @return __inline 
 */
static __inline unsigned short inports(unsigned short _port) {
	unsigned short rv;
	asm volatile("inw %1, %0"
				 : "=a"(rv)
				 : "dN"(_port));
	return rv;
}

/**
 * @brief outports
 * 
 * @param _port 
 * @param _data 
 * @return __inline 
 */
static __inline void outports(unsigned short _port, unsigned short _data) {
	asm volatile("outw %1, %0"
				 :
				 : "dN"(_port), "a"(_data));
}

/**
 * @brief inportsw
 * 
 * @param portid 
 * @param addr 
 * @param count 
 * @return __inline 
 */
static __inline void inportsw(uint16_t portid, void *addr, size_t count) {
	__asm__ __volatile__("rep insw"
						 : "+D"(addr), "+c"(count)
						 : "d"(portid)
						 : "memory");
}

/**
 * @brief outportsw
 * 
 * @param portid 
 * @param addr 
 * @param count 
 * @return __inline 
 */
static __inline void outportsw(uint16_t portid, const void *addr, size_t count) {
	__asm__ __volatile__("rep outsw"
						 : "+S"(addr), "+c"(count)
						 : "d"(portid));
}

/**
 * @brief cpuid 
 * 
 * @param code 
 * @param a 
 * @param d 
 */
void cpuid(int code, uint32_t *a, uint32_t *d);

/**
 * @brief Get the cpu vender object
 * 
 * @return const char* 
 */
const char *get_cpu_vender();

/**
 * @brief halt init function
 * 
 */
void halt_init();