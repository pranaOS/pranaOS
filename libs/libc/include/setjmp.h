#pragma once

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

#ifdef __i386__

#define _jblen (6 * 4)
#elif defined(__arm__) && defined(__ARM_NEON__)

#define _jblen (11 * 4 + 8 * 8)
#elif defined(__arm__) && !defined(__ARM_NEON__)

#define _jblen (11 * 4)
#endif

struct __jmp_buf {
    char regs[_jblen];
};

typedef struct __jmp_buf jmp_buf[1];
typedef struct __jmp_buf sigjmp_buf[1];

extern int setjmp(jmp_buf);
extern void longjmp(jmp_buf, int val);

__END_DECLS