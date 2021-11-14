#pragma once

#ifdef __cplusplus
extern "C" {
#endif

unsigned long udivmodsi4(unsigned long num, unsigned long den, int modwanted);
long          divsi3(long a, long b);
long          modsi3(long a, long b);

#ifdef __cplusplus
}
#endif