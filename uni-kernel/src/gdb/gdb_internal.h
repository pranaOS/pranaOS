/**
 * @file gdb_internal.h
 * @author Krisna Pranav
 * @brief gdb internal
 * @version 6.0
 * @date 2025-08-26
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <unix_internal.h>
#include <lwip.h>

typedef struct gdb 
{
    struct spinlock send_lock;
    string output;
    string send_buffer;
    string out;
    string in;
    heap h;
    u8 checksum, sent_checksum;
    buffer_handler output_handler;
    thread t;
    process p;
    fault_handler fault_handler;
    boolean multiprocess;
    boolean sending;
    int ctid; 
} *gdb;

typedef struct handler 
{
    char *name;

    boolean (*body)(gdb, buffer, string);
} *handler;

/**
 * @brief Get the char object
 * 
 * @param b 
 * @return char 
 */
static inline char get_char(buffer b)
{
    if (buffer_length(b)) {
        char r = peek_char(b);
        buffer_consume(b, 1);
        return(r);
    } 
    return(0);
}

/**
 * @param b 
 * @param c 
 * @return boolean 
 */
static inline boolean check(buffer b, char c)
{
    if (peek_char(b) == c) {
        get_char(b);
        return(true);
    }

    return(false);
}

/**
 * @param b 
 */
static inline void reset_buffer(string b)
{
    b->start = 0;
    b->end = 0;
}

/**
 * @param in 
 * @param first 
 * @param second 
 * @return boolean 
 */
boolean parse_hex_pair(buffer in, u64 *first, u64 *second);

/**
 * @param b 
 * @param mem 
 * @param count 
 * @return boolean 
 */
boolean mem2hex (string b, void *mem, int count);

/**
 * @param b 
 * @param mem 
 * @param count 
 * @return boolean 
 */
boolean hex2mem (buffer b, void *mem, int count);

/**
 * @param b 
 */
void putpacket(gdb, string b);

/**
 * @param b 
 */
void putpacket_deferred(gdb, string b);

/**
 * @param g 
 * @param b 
 * @param out 
 * @param h 
 * @return boolean 
 */
boolean handle_query(gdb g, buffer b, string out, handler h);

/**
 * @param h 
 * @param p 
 * @param outh 
 * @return buffer_handler 
 */
buffer_handler init_gdb(heap h,
                        process p,
                        buffer_handler outh);