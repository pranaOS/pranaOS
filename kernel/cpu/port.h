/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <ak/types.h>

namespace Kernel {

    /**
     * @brief insl
     * 
     */
    #define insl(port, buffer, count) asm volatile("cld; rep; insl" :: "D" (buffer), "d" (port), "c" (count))

    /**
     * @brief inportb
     * 
     * @param _port 
     * @return unsigned char 
     */
    inline static unsigned char inportb (unsigned short _port) {
        unsigned char rv;
        __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
        return rv;
    }

    /**
     * @brief outportb
     * 
     * @param _port 
     * @param _data 
     */
    inline static void outportb (unsigned short _port, unsigned char _data) {
        __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
    }

    /**
     * @brief inportw
     * 
     * @param _port 
     * @return unsigned short 
     */
    inline static unsigned short inportw(unsigned short _port) {
        unsigned short rv;
        __asm__ __volatile__ ("inw %1, %0" : "=a" (rv) : "dN" (_port));
        return rv;
    }

    /**
     * @brief outportw
     * 
     * @param _port 
     * @param _data 
     */
    inline static void outportw(unsigned short _port, unsigned short _data) {
        __asm__ __volatile__ ("outw %1, %0" : : "dN" (_port), "a" (_data));
    }

    /**
     * @brief inportl
     * 
     * @param _port 
     * @return unsigned int 
     */
    inline static unsigned int inportl(unsigned short _port) {
        unsigned int val;
        __asm__ __volatile__ ("inl %%dx, %%eax" : "=a" (val) : "d" (_port));
        return( val );
    }

    /**
     * @brief outportl
     * 
     * @param _port 
     * @param _data 
     */
    inline static void outportl(unsigned short _port, unsigned int _data) {
        __asm__ __volatile__ ("outl %%eax, %%dx" : : "d" (_port), "a" (_data));
    }

    /**
     * @brief inportsm
     * 
     * @param port 
     * @param data 
     * @param size 
     */
    inline static void inportsm(unsigned short port, unsigned char * data, unsigned long size) {
        asm volatile ("rep insw" : "+D" (data), "+c" (size) : "d" (port) : "memory");
    }

    /**
     * @brief outportsm
     * 
     * @param port 
     * @param data 
     * @param size 
     */
    inline static void outportsm(unsigned short port, unsigned char * data, unsigned long size) {
	    asm volatile ("rep outsw" : "+S" (data), "+c" (size) : "d" (port));
    }

    /**
     * @brief writeMem Regisiter object
     * 
     * @param addr 
     * @param val 
     */
    inline static void writeMemReg(const uint32_t addr, const uint32_t val) {
        *(volatile uint32_t*)addr = val;
    }

    /**
     * @brief read memry register object
     * 
     * @param addr 
     * @return uint32_t 
     */
    inline static uint32_t readMemReg(const uint32_t addr) {
        return *(volatile uint32_t*)addr;
    }
}