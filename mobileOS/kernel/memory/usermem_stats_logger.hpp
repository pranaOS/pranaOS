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

    /**
     * @brief Construct a new user Mem Stats Logger object
     * 
     */
    userMemStatsLogger() {
        usermemResetStatistics();
        freeHeapSize1 = usermemGetFreeHeapSize();
    }

    private:
        size_t freeHeapSize1;

#endif
}