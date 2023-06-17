/**
 * @file usermem_stats_logger.hpp
 * @author Krisna Pranav
 * @brief User Memory Stats Logger 
 * @version 1.0
 * @date 2023-06-17
 * 
 * @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <log/log.hpp>
#include "user_memory.h"

struct userMemStatsLogger {
#if DEBUG_HEAP_ALLOCATIONS == 1

    userMemStatsLogger() {
        usermemResetStatistics();
        freeHeapSize1 = usermemGetFreeHeapSize();
    }

    ~userMemStatsLogger() {
        size_t freeHeapSize2      = usermemGetFreeHeapSize();
        size_t allocationsCount   = usermemGetAllocationsCount();
        size_t deallocationsCount = usermemGetDeallocationsCount();
        size_t allocatedMin       = usermemGetAllocatedMin();
        size_t allocatedMax       = usermemGetAllocatedMax();
        size_t allocatedSum       = usermemGetAllocatedSum();
        LOG_INFO("\nFree before: %zu\nFree after: %zu\n# allocations: %zu\n# deallocations: %zu\nSmallest block: %zu\nBiggest block: %zu\nAllocated: %zu",
                 freeHeapSize1, freeHeapSize2, allocationsCount, deallocationsCount, allocatedMin, allocatedMax, allocatedSum);
    }

private:
    size_t freeHeapSize1;
#endif
};