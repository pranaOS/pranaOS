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

typedef uint32_t uintptr_t;

typedef decltype(nullptr) nullptr_t;

#define unlikely(expr) __builtin_expect(!!(expr), 0)
#define likely(expr) __builtin_expect(!!(expr), 1)

#define KB 1024
#define MB (KB * KB)

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

extern void* malloc(size_t);
extern void free(void*);

inline void* operator new(size_t size)
{
    return malloc(size);
}

inline void operator delete(void* ptr)
{
    return free(ptr);
}

inline void operator delete(void* ptr, size_t)
{
    return free(ptr);
}

inline void* operator new[](size_t size)
{
    return malloc(size);
}

inline void operator delete[](void* ptr)
{
    return free(ptr);
}

inline void operator delete[](void* ptr, size_t)
{
    return free(ptr);
}

inline void* operator new(size_t, void* ptr)
{
    return ptr;
}

inline void* operator new[](size_t, void* ptr)
{
    return ptr;
}

inline void* operator new(size_t size, std::align_val_t alignment)
{
    size_t aln = static_cast<size_t>(alignment);
    void* ptr = malloc(size + sizeof(void*) + aln);
    size_t max_addr = (size_t)ptr + sizeof(void*) + aln;
    void* aligned_ptr = (void*)(max_addr - (max_addr % aln));
    ((void**)aligned_ptr)[-1] = ptr;
    return aligned_ptr;
}