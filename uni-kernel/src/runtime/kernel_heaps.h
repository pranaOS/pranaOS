/**
 * @file kernel_heaps.h
 * @author Krisna Pranav
 * @brief Kernel Heaps
 * @version 6.0
 * @date 2025-03-27
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

typedef struct kernel_heaps {
    heap physical;
} *kernel_heaps;

/**
 * @param heaps 
 * @return heap 
 */
static inline heap heap_physical(kernel_heaps heaps)
{
    return heaps->physical;
}