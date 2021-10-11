#pragma once

#include <sys/cdefs.h>

__BEGIN_DECLS

/* define RTLD */

#define RTLD_DEFAULT 0
#define RTLD_LAZY 2
#define RTLD_NOW 4

typedef struct __Dl_info {
    const char* dli_fname;
    void* dli_fbase;
    const char* dli_sname;
    void* dli_saddr;
} Dl_info;

int dlclose(void*);

__END_DECLS