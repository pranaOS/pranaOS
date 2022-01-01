#pragma once

#include <stddef.h>
#include <stdint.h>

#define KB *1024
#define MB KB * 1024

#if __WORDSIZE == 64

typedef uint64_t uword_t;
typedef int64_t sword_t;

#else

typedef uint32_t uword_t;

#endif 

typedef unsigned long suseconds_t;

#endif