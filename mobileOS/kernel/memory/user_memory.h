/**
 * @file user_memory.h
 * @author Krisna Pranav
 * @brief UserMemory Allocations
 * @version 6.0
 * @date 2023-06-17
 * 
 * @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
 * 
 */


#ifndef USERMEMORY_H
#define USERMEMORY_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @brief usermalloc
     * 
     * @param xWantedSize 
     * @return void* 
     */
    void *usermalloc(size_t xWantedSize);

    /**
     * @brief userfree
     * 
     * @param pv 
     */
    void userfree(void *pv);

    /**
     * @brief usermemGetFreeHeapSize
     * 
     * @return size_t 
     */
    size_t usermemGetFreeHeapSize(void);

    /**
     * @brief usermemGetMinimumeverFreeHeapSize
     * 
     * @return size_t 
     */
    size_t usermemGetMinimumEverFreeHeapSize(void);

    /**
     * @brief usermemResetStatistics
     * 
     */
    void usermemResetStatistics(void);

    /**
     * @brief usermemGetAllocationsCount
     * 
     * @return size_t 
     */
    size_t usermemGetAllocationsCount(void);

    /**
     * @brief usermeeGetDeallocationsCount
     * 
     * @return size_t 
     */
    size_t usermemGetDeallocationsCount(void);

    /**
     * @brief usermemGetAllocatedMain
     * 
     * @return size_t 
     */
    size_t usermemGetAllocatedMin(void);

    /**
     * @brief usermemGetAllocatedMax
     * 
     * @return size_t 
     */
    size_t usermemGetAllocatedMax(void);
    
    /**
     * @brief usermemGetAllocatedSum
     * 
     * @return size_t 
     */
    size_t usermemGetAllocatedSum(void);

    /**
     * @brief userrealloc
     * 
     * @param pv 
     * @param xWantedSize 
     * @return void* 
     */
    void *userrealloc(void *pv, size_t xWantedSize);

#ifdef __cplusplus
}
#endif


#endif 