/**
 * @file gdb.h
 * @author Krisna Pranav
 * @brief gdb
 * @version 6.0
 * @date 2025-08-24
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#define GDB 1

/**
 * @param h 
 * @param p 
 * @param port 
 */
void init_tcp_gdb(heap h, process p, u16 port);

/**
 * @param t 
 */
void gdb_check_fault_handler(thread t);