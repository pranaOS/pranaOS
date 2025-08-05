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