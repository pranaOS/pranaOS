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
static inline int get_register(u64 num, void* buf, context_frame c)
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

/**
 * @param b 
 * @param t 
 */
static inline void read_registers(buffer b, thread t)
{
}

/**
 * @param b 
 * @param t 
 */
static inline void write_registers(buffer b, thread t)
{
}

/**
 * @brief Set the write protect object
 * 
 * @param enable 
 */
static inline void set_write_protect(boolean enable)
{
}

/**
 * @param h 
 * @param a 
 * @param type 
 * @param log_length 
 * @param completion 
 * @return boolean 
 */
boolean breakpoint_insert(heap h, u64 a, u8 type, u8 log_length, thunk completion)
{
    return false;
}

/**
 * @param h 
 * @param a 
 * @param completion 
 * @return boolean 
 */
boolean breakpoint_remove(heap h, u32 a, thunk completion)
{
    return false;
}