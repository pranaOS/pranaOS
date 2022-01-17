//
//  cpu.cpp
//  pranaOS
//
//  Created by Krisna Pranav on 13/01/22.
//

#include "cpu.h"
#include <system/console.h>

using namespace Kernel;

extern "C" void enableSSE();

static inline void cpuid(uint32_t reg, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx) {
    asm volatile("cpuid"
        : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx)
        : "0" (reg));
}
