/**
 * @file non_cached_mem_allocations.hpp
 * @author Krisna Pranav
 * @brief NonCachedMemAllocations
 * @version 0.1
 * @date 2023-06-17
 * 
 * @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <cstdint>
#include <cassert>
#include <FreeRTOS.h>

/**
 * @brief nonCachedMemAllocator
 * 
 * @tparam T 
 */
template <typename T> struct nonCachedMemAllocator {
    using value_type = T;

    T *allocate(std::size_t num);
    void deallocate(T *ptr, std::size_t num);

    nonCachedMemAllocator()                              = default;
    nonCachedMemAllocator(const nonCachedMemAllocator &) = default;
    nonCachedMemAllocator(nonCachedMemAllocator &&)      = default;

    nonCachedMemAllocator &operator=(const nonCachedMemAllocator &) = default;
    nonCachedMemAllocator &operator=(nonCachedMemAllocator &&) = default;
};

/**
 * @brief allocate
 * 
 * @tparam T 
 * @param num 
 * @return T* 
 */
template <typename T> T *nonCachedMemAllocator<T>::allocate(std::size_t num) {
    T *ptr = reinterpret_cast<T *>(pvPortMalloc(sizeof(T) * num));
    return ptr;
}

/**
 * @brief deallocate
 * 
 * @tparam T 
 * @param ptr 
 */
template <typename T> void nonCachedMemAllocator<T>::deallocate(T *ptr, std::size_t) {
    assert(ptr != nullptr);
    vPortFree(ptr);
}