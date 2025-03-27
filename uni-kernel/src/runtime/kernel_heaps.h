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

typedef struct kernel_heaps
{
    heap physical;

    id_heap virtual_huge;
    id_heap virtual_page;

    backed_heap page_backed;

    backed_heap linear_backed;

    heap pages;

    heap general;

    heap locked;

    heap malloc;

    heap dma;
}* kernel_heaps;

/**
 * @param heaps 
 * @return heap 
 */
static inline heap heap_physical(kernel_heaps heaps)
{
    return heaps->physical;
}

/**
 * @param heaps 
 * @return id_heap 
 */
static inline id_heap heap_virtual_huge(kernel_heaps heaps)
{
    return heaps->virtual_huge;
}

/**
 * @param heaps 
 * @return id_heap 
 */
static inline id_heap heap_virtual_page(kernel_heaps heaps)
{
    return heaps->virtual_page;
}

/**
 * @param heaps 
 * @return backed_heap 
 */
static inline backed_heap heap_page_backed(kernel_heaps heaps)
{
    return heaps->page_backed;
}

/**
 * @param heaps 
 * @return backed_heap 
 */
static inline backed_heap heap_linear_backed(kernel_heaps heaps)
{
    return heaps->linear_backed;
}

/**
 * @param heaps 
 * @return heap 
 */
static inline heap heap_general(kernel_heaps heaps)
{
    return heaps->general;
}

/**
 * @param heaps 
 * @return heap 
 */
static inline heap heap_locked(kernel_heaps heaps)
{
    return heaps->locked;
}