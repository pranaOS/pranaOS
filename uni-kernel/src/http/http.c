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

/**
 * @param h
 * @param bh
 * @param method
 * @param headers
 * @param body
 * @return status
 */
status http_request(heap h, buffer_handler bh, http_method method, tuple headers, buffer body)
{
    buffer b = allocate_buffer(h, 100);
    buffer url = get(headers, sym(url));
    bprintf(b, "%s %b HTTP/1.1\r\n", http_request_methods[method], url);
    buffer content_len = little_stack_buffer(16);
    bprintf(content_len, "%ld", body ? buffer_length(body) : 0);
    set(headers, sym(Content - Length), content_len);
    http_header(b, headers);
    status s = apply(bh, b);
    if(!is_ok(s))
    {
        deallocate_buffer(b);
        return timm_up(s, "result", "%s failed to send", func_ss);
    }
    if(body)
        s = apply(bh, body);
    return s;
}

/**
 * @param out
 * @param t
 * @return status
 */
static status send_http_headers(http_responder out, tuple t)
{
    status s;

    if(out->keepalive && out->http_version <= HTTP_VER(1, 0))
        set(t, sym(connection), aprintf(out->h, "keep-alive"));
    else if(!out->keepalive && out->http_version >= HTTP_VER(1, 1))
        set(t, sym(connection), aprintf(out->h, "close"));

    buffer d = allocate_buffer(transient, 128);
    bprintf(d, "HTTP/%d.%d ", HTTP_MAJ(out->http_version), HTTP_MIN(out->http_version));

    symbol ss = sym(status);
    string sstr = get_string(t, ss);

    if(sstr)
        bprintf(d, "%b\r\n", sstr);
    else
        bprintf(d, "200 OK\r\n");

    iterate(t, stack_closure(each_header, d, ss, true));
    deallocate_value(t);
    bprintf(d, "\r\n");

    s = apply(out->out, d);

    if(!is_ok(s))
    {
        deallocate_buffer(d);
        return timm_up(s, "result", "%s failed to send", func_ss);
    }

    return STATUS_OK;
}

void http_register_uri_handler(http_listener hl, sstring uri, http_request_handler each)
{
    http_listener_registrant r = allocate(hl->h, sizeof(struct http_listener_registrant));
    assert(r != INVALID_ADDRESS);
    r->uri = uri;
    r->each = each;
    list_insert_before(&hl->registrants, &r->l);
}

void http_register_default_handler(http_listener hl, http_request_handler each)
{
    hl->default_handler = each;
}

connection_handler connection_handler_from_http_listener(http_listener hl)
{
    return closure(hl->h, each_http_connection, hl);
}

http_listener allocate_http_listener(heap h, u16 port)
{
    http_listener hl = allocate(h, sizeof(struct http_listener));
    if(hl == INVALID_ADDRESS)
        return hl;

    hl->h = h;
    hl->default_handler = 0;
    list_init(&hl->registrants);
    return hl;
}

void deallocate_http_listener(heap h, http_listener hl)
{
    deallocate(h, hl, sizeof(struct http_listener));
}