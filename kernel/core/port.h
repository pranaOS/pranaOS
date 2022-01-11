#pragma once

#include <ak/types.h>

namespace Kernel {
    namespace core {
        #define insl(port, buffer, count) asm volatile("cld; rep; insl" :: "D" (buffer), "d" (port), "c" (count))

        inline static unsigned char inportb (unsigned short _port)
        {
            unsigned char rv;
            __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
            return rv;
        }

        inline static void outportb (unsigned short _port, unsigned char _data)
        {
            __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
        }

        inline static unsigned short inportw(unsigned short _port)
        {
            unsigned short rv;
            __asm__ __volatile__ ("inw %1, %0" : "=a" (rv) : "dN" (_port));
            return rv;
        }

        inline static void outportw(unsigned short _port, unsigned short _data)
        {
            __asm__ __volatile__ ("outw %1, %0" : : "dN" (_port), "a" (_data));
        }
        inline static unsigned int inportl(unsigned short _port)
        {
            unsigned int val;
            __asm__ __volatile__ ("inl %%dx, %%eax" : "=a" (val) : "d" (_port));
            return( val );
        }
        inline static void outportl(unsigned short _port, unsigned int _data)
        {
            __asm__ __volatile__ ("outl %%eax, %%dx" : : "d" (_port), "a" (_data));
        }

        inline static void inportsm(unsigned short port, unsigned char * data, unsigned long size)
        {
            asm volatile ("rep insw" : "+D" (data), "+c" (size) : "d" (port) : "memory");
        }

        inline static void outportsm(unsigned short port, unsigned char * data, unsigned long size) {
	        asm volatile ("rep outsw" : "+S" (data), "+c" (size) : "d" (port));
        }

        inline static void writeMemReg(const ak::uint32_t addr, const ak::uint32_t val) {
            *(volatile ak::uint32_t*)addr = val;
        }

        inline static ak::uint32_t readMemReg(const ak::uint32_t addr) {
            return *(volatile ak::uint32_t*)addr;
        }
    }
}