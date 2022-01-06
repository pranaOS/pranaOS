#include "memoperator.h"

using namespace pranaOS::ak;

void* MemOperator::memmove(void* dstptr, const void* srcptr, uint32_t size) {
    unsigned char* dst = (unsigned char*) dstptr;
    const unsigned char* src = (const unsigned char*) srcptr;
    if (dst < src) {
        for (uint32_t i = 0; i < size; i++) 
            dst[i] = src[i]
    } else {
        for (uint32_t i = size; i != 0; i--)
            dst[i-1] = src[i-1];
    }

    return dstptr;
}