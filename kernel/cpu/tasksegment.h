#pragma once

#include "gdtentry.h"
#include <ak/types.h>
#include <ak/memoryoperations.h>

namespace Kernel {

    struct tssEntry {
        ak::uint32_t prevTss;
        ak::uint32_t esp0;
        ak::uint32_t ss0;
        ak::uint32_t esp1;
        ak::uint32_t ss1;
        ak::uint32_t esp2;
        ak::uint32_t ss2;
        ak::uint32_t cr3;
        ak::uint32_t eip;
        ak::uint32_t eflags;
        ak::uint32_t eax;
        ak::uint32_t ecx;
        ak::uint32_t edx;
        ak::uint32_t ebx;
        ak::uint32_t esp;
        ak::uint32_t ebp;
        ak::uint32_t esi;
        ak::uint32_t edi;
        ak::uint32_t es;
        ak::uint32_t cs;
        ak::uint32_t ss;
        ak::uint32_t ds;
        ak::uint32_t fs;
        ak::uint32_t gs;
        ak::uint32_t ldt;
        ak::uint16_t trap;
        ak::uint16_t iomap;
    };

        class TSS {
        public:
            static void install(ak::uint32_t idx, ak::uint32_t kernelSS, ak::uint32_t kernelESP);
            static void setStack(ak::uint32_t kernelSS, ak::uint32_t kernelESP);
            static tssEntry* getCurrent();
        };
    }
}