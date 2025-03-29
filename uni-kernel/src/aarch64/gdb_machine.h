/**
 * @file gdb_machine.h
 * @author Krisna Pranav
 * @brief gdb machine
 * @version 6.0
 * @date 2025-03-29
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

/**
 * @param c 
 * @return int 
 */
static inline int computeSignal(context_frame c)
{
    return 0;
}

/**
 * @brief Get the register object
 * 
 * @param num 
 * @param buf 
 * @param c 
 * @return int 
 */
static inline int get_register(u64 num, void *buf, context_frame c)
{
    return -1;
}

/**
 * @brief Set the thread register object
 * 
 * @param t 
 * @param regno 
 * @param val 
 * @return boolean 
 */
static boolean set_thread_register(thread t, int regno, u64 val)
{
    return false;
}

/**
 * @brief Set the thread pc object
 * 
 * @param t 
 * @param addr 
 */
static inline void set_thread_pc(thread t, u64 addr)
{
}