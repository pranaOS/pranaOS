/**
 * @file unix_process_runtime.h
 * @author Krisna Pranav
 * @brief Unix Process Runtime
 * @version 6.0
 * @date 2025-07-29
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
*/

typedef int descriptor;

heap init_process_runtime();

/**
 * @return sstring 
 */
sstring errno_sstring(void);

/**
 * @param meta 
 * @param size 
 * @return heap 
 */
heap allocate_mmapheap(heap meta, bytes size);