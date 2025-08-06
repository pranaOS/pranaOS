/**
 * @file http.c
 * @author Krisna Pranav
 * @brief http
 * @version 6.0
 * @date 2025-08-05
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#include <http.h>
#include <runtime.h>

#define HTTP_VER(x, y) (((x) << 16) | ((y) & MASK(16)))
#define HTTP_MAJ(v) ((v) >> 16)
#define HTTP_MIN(v) ((v) & MASK(16))

#define STATE_INIT 0
#define STATE_START_LINE 1
#define STATE_HEADER 2
#define STATE_VALUE 3
#define STATE_BODY 4

typedef struct http_parser
{
    heap h;
    vector start_line;
    int state;
    buffer word;
    symbol s;
    tuple header;
    value_handler each;
    u64 content_length;
}* http_parser;

struct http_responder
{
    heap h;
    buffer_handler out;
    u32 http_version;
    boolean keepalive;
};

closure_function(3, 2, boolean, each_header,
                 buffer, dest, symbol, ignore, boolean, dealloc,
                 value n, value v)
{
    if(n != bound(ignore))
    {
        if(is_tuple(v) || is_symbol(v))
            bprintf(bound(dest), "%v: %v\r\n", n, v);
        else
            bprintf(bound(dest), "%v: %b\r\n", n, (buffer)v);
    }

    if(bound(dealloc))
    {
        assert(!is_tuple(v));
        deallocate_value(v);
    }
    return true;
}

/**
 * @param dest
 * @param t
 */
static void http_header(buffer dest, tuple t)
{
    iterate(t, stack_closure(each_header, dest, sym(url), false));
    bprintf(dest, "\r\n");
}