/**
 * @file unix_process.h
 * @author Krisna Pranav
 * @version 6.0
 * @date 2025-06-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


typedef int descriptor;

/**
 * @return heap 
 */
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

/**
 * @param parent 
 * @return heap 
 */
heap make_tiny_heap(heap parent);

/**
 * @param h 
 * @param argc 
 * @param argv 
 * @return tuple 
 */
tuple parse_arguments(heap h, int argc, char **argv);