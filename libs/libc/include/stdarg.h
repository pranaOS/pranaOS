#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef char *va_list;

#define __va_SIZE(TYPE)                                                        \
    (((sizeof(TYPE) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))

#define va_start(AP, LASTARG)                                                  \
    (AP = ((va_list) & (LASTARG) + __va_SIZE(LASTARG)))

#define va_arg(AP, TYPE)                                                       \
    (AP += __va_SIZE(TYPE), *((TYPE *)(AP - __va_SIZE(TYPE))))

#define va_end(AP) (AP = (va_list)0)

#ifdef __cplusplus
}
#endif
