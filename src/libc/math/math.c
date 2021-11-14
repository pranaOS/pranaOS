#ifndef __cplusplus
extern "C" {
#endif

#if defined(__arm__) || defined(__aarch64__)

#include "math.h"

long modsi3(long a, long b) {
    int  neg = 0;
    long res;
    if (a < 0) {
        a   = -a;
        neg = 1;
    }
    if (b < 0) {
        b = -b;
    }
    res = udivmodsi4(a, b, 1);
    if (neg) {
        res = -res;
    }
    return res;
}

#endif

#ifndef __cplusplus
}
#endif