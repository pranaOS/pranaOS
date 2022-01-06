#pragma once

namespace Types {
    #define STATIC_ASSERT(condition) typedef char p__LINE__[ (condition) ? 1 : -1];

    typedef char                        int8_t;
    typedef unsigned char               uint8_t;
    typedef short                       int16_t;
    typedef unsigned short              uint16_t;
    typedef int                         int32_t;
    typedef unsigned int                uint32_t;
    typedef long long int               int64_t;
    typedef unsigned long long int      uint64_t;
    typedef unsigned long long          uintptr_t;

}