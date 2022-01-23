//
// Created by KrisnaPranav on 23/01/22.
//

#include "fpu.h"
#include <ak/memoperator.h>

using namespace Kernel;
using namespace ak;


void Fpu::enable() {
    uint32_t cr4;

    asm volatile ("mov %%cr4, %0" : "=r"(cr4));
    cr4 |= 0x200;
    asm volatile ("mov %0, %%cr4" :: "r"(cr4));

    fpuControlWord cw;
    memOperator::memset(&cw, 0, sizeof(fpuControlWord));

    cw.InvalidOperand = 1;
    cw.DenormalOperand = 1;
    cw.ZeroDevide = 1;
    cw.Overflow = 1;
    cw.Underflow = 1;
    cw.Precision = 1;

    cw.PrecisionControl = 0b11;
    cw.RoundingControl = 0b00;
    cw.InfinityControl = 0;

    asm volatile("fldcw %0" :: "m"(cw));
}