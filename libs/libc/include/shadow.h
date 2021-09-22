#pragma once

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

struct spwd {
    char* sp_namp;
    char* sp_pwdp;
    int sp_lstchg;
    int sp_min;
    int sp_warn;
    int sp_inact;
    int sp_expire;
    uint32_t sp_flag;
};

typedef struct spwd spwd_t;

void setspent();

__END_DECLS