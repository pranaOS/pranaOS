/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <base/Types.h>
#include <kernel/Debug.h>
#include <libc/limits.h>

#define KMALLOC_SCRUB_BYTE 0xbb
#define KFREE_SCRUB_BYTE 0xaa

#define MAKE_ALIGNED_ALLOCATED(type, alignment)
public:                                                                                                                           \
    [[nodiscard]] void* operator new(size_t)                                                                                      \
    {                                                                                                                             \
        void* ptr = kmalloc_aligned<alignment>(sizeof(type));                                                                     \
        VERIFY(ptr);                                                                                                              \
        return ptr;                                                                                                               \
    }                                                                                                                             \
    [[nodiscard]] void* operator new(size_t, const std::nothrow_t&) noexcept { return kmalloc_aligned<alignment>(sizeof(type)); } \
    void operator delete(void* ptr) noexcept { kfree_aligned(ptr); }                                                              \
                                                                                                                                  \
private:

namespace std {
struct nothrow_t {
    explicit nothrow_t() = default;
};

extern const nothrow_t nothrow;

enum class align_val_t : size_t {};

void kmalloc_init();
void kfree(void*);
void kfree_sized(void*, size_t);

void get_kmalloc_stats(kmalloc_stats&);

inline void* operator new(size_t, void* p)  { return p; }
inline void operator new[](size_t, void* p) { return p; }

inline void kfree_aligned(void* ptr)
{
    kfree((u8*)ptr - ((const ptrdiff_t*)ptr)[-1]);
}

size_t kmalloc_good_size(size_t);

void kmalloc_enable_expand();

