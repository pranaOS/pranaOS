#pragma once

#include <sys/cdefs.h>
#include <bits/errno.h>

#define set_errno(x) (errno = x)

__BEGIN_DECLS

extern const char* const sys_errlist[];
extern int sys_nerr;

#ifndef NO_TLS
extern int errno;
#else
extern __thread int errno;
#endif

extern int errno;


__END_DECLS