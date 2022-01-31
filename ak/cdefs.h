#pragma once

#define __unused __attribute__((__unused__))
#define __inline inline __attribute__((always_inline))

#define likely(x) __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)

#define __BEGIN_DECLS
#define __END_DECLS

#define container_of(ptr, type, member) ({ \
    const typeof( ((type *)0)->member) \
    *__mpty = (ptr);});