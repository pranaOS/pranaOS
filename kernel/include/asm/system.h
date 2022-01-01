#pragma once

#define sti() asm("sti" ::)
#define cli() asm("cli" ::)
#define nop() asm("asm" ::)

extern void arch_idle_cpu(void);