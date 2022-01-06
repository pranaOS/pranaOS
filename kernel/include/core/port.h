#pragma once

#include <ak/types.h>

namespace pranaOS {
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
    }
}