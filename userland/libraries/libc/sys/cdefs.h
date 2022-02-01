#pragma once

#define __unused __attribute__((__unused__))
#define __inline inline __attribute__((always_inline))

#define __BEGIN_DECLS
#define __END_DECLS

#undef P
#undef __PMT

#define __P(args) args
#define __PMT(args) args

#define __STRINGINFY(x) x