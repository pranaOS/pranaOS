#ifndef _LIBC_SYS_CDEFS_H
#define _LIBC_SYS_CDEFS_H

#if defined(__cplusplus)
#define __BEGIN_DECLS extern "C" {
#define __END_DECLS }
#else
#define __BEGIN_DECLS
#define __END_DECLS
#endif

#ifndef __use_instead
#ifdef __clang__
#define __use_instead(F) __attribute__((diagnose_if(1, "use " F " instead", \
    "warning")))
#elif defined(__GNUC__)
#define __use_instead(F) __attribute__((warning("use " F " instead")))
#endif
#endif

#endif 