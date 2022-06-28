//
//  Common.hpp
//  Libraries
//
//  Created by Pranav on 28/06/22.
//

#pragma once

typedef unsigned long uint64_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

typedef long int64_t;
typedef signed int int32_t;
typedef signed short int16_t;
typedef signed char int8_t;

#ifdef __i386__
typedef uint64_t size_t;
#else
typedef uint32_t size_t;
#endif

inline void* operator new(size_t size) {
    return malloc(size);
}
