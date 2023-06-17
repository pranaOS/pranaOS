/**
 * @file user_memory.h
 * @author Krisna Pranav
 * @brief UserMemory Allocations
 * @version 1.0
 * @date 2023-06-17
 * 
 * @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
 * 
 */

#ifndef USERMEMORY_H
#define USERMEMORY_H

#include <stdlib.h>

#ifndef __cplusplus 
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
     * @brief userFree
     * 
     * @param pv 
     */
    void userFree(void *pv);

    /**
     * @brief userMemGetFreeHeapSize
     * 
     * @return size_t 
     */
    size_t usermemGetFreeHeapSize(void);

    /**
     * @brief usermemGetMinimumEverFreeHeapSize
     * 
     * @return size_t 
     */
    size_t usermemGetMinimumEverFreeHeapSize(void);

    /**
     * @brief userrealloc
     * 
     * @param pv 
     * @param xWantedSize 
     * @return void* 
     */
    void *userrealloc(void *pv, size_t xWantedSize);

#ifndef __cplusplus
}
#endif 

#endif 