#pragma once

#define std() asm("std" ::)

extern void arch_idle_cpu(void);