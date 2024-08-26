/**
 * @file kmalloc.h
 * @author Krisna Pranav
 * @brief kmalloc
 * @version 6.0
 * @date 2023-06-29
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>
#include <libc/limits.h>

// YUM YUM BYTES :)
#define KMALLOC_SCRUB_BYTE 0xbb
#define KFREE_SCRUB_BYTE 0xaa

#define MAKE_ALIGNED_ALLOCATED(type, alignment)                                                                                   \
public:                                                                                                                           \
    [[nodiscard]] void* operator new(size_t)                                                                                      \
    {                                                                                                                             \
        void* ptr = kmalloc_aligned(sizeof(type), alignment);                                                                     \
        VERIFY(ptr);                                                                                                              \
        return ptr;                                                                                                               \
    }                                                                                                                             \
    [[nodiscard]] void* operator new(size_t, const std::nothrow_t&) noexcept { return kmalloc_aligned(sizeof(type), alignment); } \
    void operator delete(void* ptr) noexcept { kfree_aligned(ptr); }                                                              \
                                                                                                                                  \
private:

namespace std 
{ 
    struct nothrow_t 
    {  
        /**
         * @brief Construct a new nothrow_t object
         * 
         */
        explicit nothrow_t() = default;
    }; // struct nothrow_t

    extern const nothrow_t nothrow;

    enum class align_val_t : size_t {};
}; // namespace std

void kmalloc_init();

void kfree_sized(void*, size_t);

struct kmalloc_stats 
{
    size_t bytes_allocated;
    size_t bytes_free;
    size_t kmalloc_call_count;
    size_t kfree_call_count;
}; // struct kmalloc_stats

void get_kmalloc_stats(kmalloc_stats&);

extern bool g_dump_kmalloc_stacks;

/**
 * @param p 
 * @return void* 
 */
inline void* operator new(size_t, void* p) 
{ 
    return p; 
}

/**
 * @param p 
 * @return void* 
 */
inline void* operator new[](size_t, void* p) 
{ 
    return p; 
}

/**
 * @param size 
 * @return void* 
 */
[[nodiscard]] void* operator new(size_t size);

/**
 * @param size 
 * @return void* 
 */
[[nodiscard]] void* operator new(size_t size, const std::nothrow_t&) noexcept;

/**
 * @param size 
 * @return void* 
 */
[[nodiscard]] void* operator new(size_t size, std::align_val_t);

/**
 * @param size 
 * @return void* 
 */
[[nodiscard]] void* operator new(size_t size, std::align_val_t, const std::nothrow_t&) noexcept;

void operator delete(void* ptr) noexcept DISALLOW("All deletes in the kernel has known size.");

/**
 * @param ptr 
 */
void operator delete(void* ptr, size_t) noexcept;

void operator delete(void* ptr, std::align_val_t) noexcept DISALLOW("All deletes in the kernel has known size.");

/**
 * @param ptr 
 */
void operator delete(void* ptr, size_t, std::align_val_t) noexcept;

/**
 * @param size 
 * @return void* 
 */
[[nodiscard]] void* operator new[](size_t size);

/**
 * @param size 
 * @return void* 
 */
[[nodiscard]] void* operator new[](size_t size, const std::nothrow_t&) noexcept;

void operator delete[](void* ptrs) noexcept DISALLOW("All deletes in the kernel has known size.");

/**
 * @param ptr 
 */
void operator delete[](void* ptr, size_t) noexcept;

/**
 * @return void* 
 */
[[gnu::malloc, gnu::alloc_size(1)]] void* kmalloc(size_t);

/**
 * @param size 
 * @param alignment 
 * @return void* 
 */
[[gnu::malloc, gnu::alloc_size(1), gnu::alloc_align(2)]] void* kmalloc_aligned(size_t size, size_t alignment);

/**
 * @param ptr 
 */
inline void kfree_aligned(void* ptr)
{
    if (ptr == nullptr)
        return;
    kfree_sized((u8*)ptr - ((ptrdiff_t const*)ptr)[-1], ((size_t const*)ptr)[-2]);
} // inline void kfree_aligned

/**
 * @return size_t 
 */
size_t kmalloc_good_size(size_t);

void kmalloc_enable_expand();
